"use strict";
/* 本文件由 tests/build-engine.js 覆盖到 engine-src（基于 JSCPP 2.0.9 lib/includes/cstdio.js 修补）：
 * 1. scanf 支持 %lf/%lld/%lli/%ld/%lu/%llu/%hd/%i（原来 %lf/%lld 直接抛 Unknown type）
 * 2. %d/%u 一律十进制（原实现把 "0" 开头按八进制解析，"08" 会报 invalid octal）；%i 保持自动识别
 * 3. scanf 输入推进改用 match 下标（原 indexOf(result) 在重复子串时会错位）
 * 4. %c 正确写入单字符变量（原实现把字符串塞进数值格导致 Memory overflow/NaN）
 * 5. printf 的格式串支持长度修饰符 %ld/%lld/%lf（原来 printf("%ld") 报 insufficient arguments）
 * 6. puts 输出补换行（C 标准 puts 自带换行）
 * 7. sprintf 重写（原实现调用 _strcpy 的方式错误，必报 "destination or source is not an array"）
 * 8. 注册 EOF / NULL 常量；getchar 读到末尾返回 EOF(-1 与 C 常见实现一致)
 * 9. %f 读取正则支持 .5 / 1e3 / +3 等写法
 */
const printf = require("printf");
/* 与标准 C 一致：EOF = -1 */
const EOF = -1;
const NULL = -1;
const format_type_map = function (rt, conv, mod) {
    switch (conv) {
        case "d":
        case "i":
            if (mod === "ll" || mod === "j") return rt.primitiveType("long long int");
            if (mod === "l" || mod === "t" || mod === "z") return rt.primitiveType("long int");
            if (mod === "h" || mod === "hh") return rt.primitiveType("short int");
            return rt.intTypeLiteral;
        case "u":
        case "o":
        case "x":
        case "X":
            if (mod === "ll" || mod === "j") return rt.primitiveType("unsigned long long int");
            if (mod === "l" || mod === "t" || mod === "z") return rt.primitiveType("unsigned long int");
            if (mod === "h" || mod === "hh") return rt.primitiveType("unsigned short int");
            return rt.unsignedintTypeLiteral;
        case "f":
        case "F":
            return rt.floatTypeLiteral;
        case "e":
        case "E":
        case "g":
        case "G":
        case "a":
        case "A":
            return rt.doubleTypeLiteral;
        case "c":
            return rt.charTypeLiteral;
        case "s":
            return rt.normalPointerType(rt.charTypeLiteral);
        case "p":
            return rt.normalPointerType(rt.voidTypeLiteral);
        case "n":
            rt.raiseException("%n is not supported");
    }
};
const validate_format = function (rt, format, ...params) {
    let i = 0;
    /* 支持长度修饰符 hh/h/ll/l/L/z/j/t（%ld、%lld、%lf 等） */
    const re = /%(?:[-+ #0])?(?:[0-9]+|\*)?(?:\.(?:[0-9]+|\*))?(?:hh|h|ll|l|L|z|j|t)?([diuoxXfFeEgGaAcspn])/g;
    return (() => {
        let ctrl;
        const result = [];
        while ((ctrl = re.exec(format)) != null) {
            const modMatch = /(?:hh|h|ll|l|L|z|j|t)(?=[diuoxXfFeEgGaAcspn])$/.exec(ctrl[0].slice(1));
            const type = format_type_map(rt, ctrl[1], modMatch ? modMatch[0] : null);
            if (params.length <= i) {
                rt.raiseException(`insufficient arguments (at least ${i + 1} is required)`);
            }
            const target = params[i++];
            const casted = rt.cast(type, target);
            if (rt.isStringType(casted)) {
                result.push(rt.getStringFromCharArray(casted));
            }
            else {
                if (casted.v == null || (typeof (casted.v) === "number" && isNaN(casted.v))) {
                    rt.raiseException("uninitialized value when using printf");
                }
                result.push(casted.v);
            }
        }
        return result;
    })();
};
function __range__(left, right, inclusive) {
    const range = [];
    const ascending = left < right;
    const end = !inclusive ? right : ascending ? right + 1 : right - 1;
    for (let i = left; ascending ? i < end : i > end; ascending ? i++ : i--) {
        range.push(i);
    }
    return range;
}
module.exports = {
    load(rt) {
        const char_pointer = rt.normalPointerType(rt.charTypeLiteral);
        const { stdio } = rt.config;
        /* PATCH: 输入缓冲挂到 rt 上与 <iostream> 共享——混用 scanf 与 cin 读同一份数据
         * 不会各自从头重读（原来两个头文件各持独立副本，第二个会从开头重复读） */
        if (!rt.__stdinBox) {
            rt.__stdinBox = { text: stdio.drain() };
        }
        /* 注册 EOF / NULL（stdio.h 里的宏），getchar/文件读判末尾要用 */
        rt.scope[0].variables["EOF"] = rt.val(rt.intTypeLiteral, EOF);
        rt.scope[0].variables["NULL"] = rt.val(char_pointer, rt.nullPointerValue);
        const _consume_next_char = function () {
            let char_return = "";
            if (rt.__stdinBox.text.length > 0) {
                char_return = rt.__stdinBox.text[0];
                rt.__stdinBox.text = rt.__stdinBox.text.substr(1);
                return char_return;
            }
            else {
                throw new Error("EOF");
            }
        };
        const _consume_next_line = function () {
            let retval;
            const next_line_break = rt.__stdinBox.text.indexOf('\n');
            if (next_line_break > -1) {
                retval = rt.__stdinBox.text.substr(0, next_line_break);
                rt.__stdinBox.text = rt.__stdinBox.text.replace(`${retval}\n`, '');
            }
            else {
                retval = rt.__stdinBox.text;
                rt.__stdinBox.text = "";
            }
            return retval;
        };
        const _strcpy = require("./shared/cstring_strcpy");
        const __printf = function (format, ...params) {
            if (rt.isStringType(format.t)) {
                const formatStr = rt.getStringFromCharArray(format);
                const parsed_params = validate_format(rt, formatStr, ...params);
                /* 底层 printf 包不认识 %ld/%lld/%lf 等长度修饰符（会原样输出），
                 * 值在 JS 里本来就是 Number，先剥掉修饰符再交给它格式化 */
                const plain = formatStr.replace(/%([-+ #0]*)([0-9]+|\*)?(\.(?:[0-9]+|\*))?(?:hh|h|ll|l|L|z|j|t)?/g, '%$1$2$3');
                const retval = printf(plain, ...parsed_params);
                return rt.makeCharArrayFromString(retval);
            }
            else {
                rt.raiseException("format must be a string");
            }
        };
        const _sprintf = function (rt, _this, target, format, ...params) {
            const retval = __printf(format, ...params);
            const s = rt.getStringFromCharArray(retval);
            /* 写入 target 指向的字符数组（支持数组与带偏移的指针） */
            let cells;
            let start;
            if (rt.isArrayType(target.t)) {
                cells = target.v.target;
                start = target.v.position || 0;
            }
            else if (rt.isNormalPointerType(target.t) && Array.isArray(target.v.target)) {
                cells = target.v.target;
                start = target.v.position || 0;
            }
            else if (rt.isNormalPointerType(target.t) && target.v.target && typeof target.v.target.v !== "undefined") {
                cells = [target.v.target];
                start = 0;
            }
            else {
                rt.raiseException("sprintf destination is not a char pointer");
            }
            if (start + s.length + 1 > cells.length) {
                rt.raiseException("sprintf destination is not big enough");
            }
            for (let i = 0; i < s.length; i++) {
                cells[start + i] = rt.val(rt.charTypeLiteral, s.charCodeAt(i));
            }
            cells[start + s.length] = rt.val(rt.charTypeLiteral, 0);
            return rt.val(rt.intTypeLiteral, s.length);
        };
        rt.regFunc(_sprintf, "global", "sprintf", [char_pointer, char_pointer, "?"], rt.intTypeLiteral);
        const _printf = function (rt, _this, format, ...params) {
            const retval = __printf(format, ...params);
            const retvalStr = rt.getStringFromCharArray(retval);
            stdio.write(retvalStr);
            return rt.val(rt.intTypeLiteral, retval.v.target.length);
        };
        rt.regFunc(_printf, "global", "printf", [char_pointer, "?"], rt.intTypeLiteral);
        const _getchar = function (rt, _this) {
            try {
                const char = _consume_next_char();
                return rt.val(rt.intTypeLiteral, char.charCodeAt(0));
            }
            catch (error) {
                return rt.val(rt.intTypeLiteral, EOF);
            }
        };
        rt.regFunc(_getchar, "global", "getchar", [], rt.intTypeLiteral);
        const _gets = function (rt, _this, charPtr) {
            const return_value = _consume_next_line();
            const destArray = charPtr.v.target;
            for (let i = 0, end = return_value.length, asc = 0 <= end; asc ? i <= end : i >= end; asc ? i++ : i--) {
                try {
                    destArray[i] = rt.val(rt.charTypeLiteral, return_value.charCodeAt(i));
                }
                catch (error) {
                    destArray[i] = rt.val(rt.charTypeLiteral, 0);
                }
            }
            destArray[return_value.length] = rt.val(rt.charTypeLiteral, 0);
            return rt.val(char_pointer, charPtr.v);
        };
        rt.regFunc(_gets, "global", "gets", [char_pointer], char_pointer);
        const _putchar = function (rt, _this, char) {
            const print_mask = rt.makeCharArrayFromString("%c");
            _printf(rt, null, print_mask, char);
            return char;
        };
        rt.regFunc(_putchar, "global", "putchar", [rt.charTypeLiteral], rt.intTypeLiteral);
        const _puts = function (rt, _this, charPtr) {
            const print_mask = rt.makeCharArrayFromString("%s");
            _printf(rt, null, print_mask, charPtr);
            /* C 标准：puts 输出字符串后自动换行 */
            stdio.write("\n");
            return rt.val(rt.intTypeLiteral, 1);
        };
        rt.regFunc(_puts, "global", "puts", [char_pointer], rt.intTypeLiteral);
        const _ASCII = {
            a: 'a'.charCodeAt(0),
            f: 'f'.charCodeAt(0),
            A: 'A'.charCodeAt(0),
            F: 'F'.charCodeAt(0),
            0: '0'.charCodeAt(0),
            8: '8'.charCodeAt(0),
            9: '9'.charCodeAt(0)
        };
        const _hex2int = function (str) {
            let ret = 0;
            let digit = 0;
            str = str.replace(/^[0O][Xx]/, '');
            for (let i = str.length - 1; i >= 0; i--) {
                const num = _int_at_hex(str[i], digit++);
                if (num !== null) {
                    ret += num;
                }
                else {
                    throw new Error('invalid hex ' + str);
                }
            }
            function _int_at_hex(c, digit) {
                let ret;
                const ascii = c.charCodeAt(0);
                if ((_ASCII.a <= ascii) && (ascii <= _ASCII.f)) {
                    ret = ascii - _ASCII.a + 10;
                }
                else if ((_ASCII.A <= ascii) && (ascii <= _ASCII.F)) {
                    ret = ascii - _ASCII.A + 10;
                }
                else if ((_ASCII[0] < ascii) && (ascii <= _ASCII[9])) {
                    ret = ascii - _ASCII[0];
                }
                else {
                    throw new Error(`Invalid ascii [${c}]`);
                }
                ret *= Math.pow(16, digit);
                return ret;
            }
            ;
            return ret;
        };
        const _octal2int = function (str) {
            str = str.replace(/^0/, '');
            let ret = 0;
            let digit = 0;
            for (let i = str.length - 1; i >= 0; i--) {
                const num = _int_at_octal(str[i], digit++);
                if (num !== null) {
                    ret += num;
                }
                else {
                    throw new Error(`invalid octal ${str}`);
                }
            }
            function _int_at_octal(c, digit) {
                let num = null;
                const ascii = c.charCodeAt(0);
                if ((ascii >= _ASCII[0]) && (ascii <= _ASCII[8])) {
                    num = ascii - _ASCII[0];
                }
                else {
                    throw new Error(`invalid char at [${c}]`);
                }
                num *= Math.pow(8, digit);
                return num;
            }
            ;
            return ret;
        };
        const _regslashs = (pre) => pre.replace(/\[/g, '\\[').replace(/\]/g, '\\]').replace(/\(/g, '\\(').replace(/\)/g, '\\)').replace(/\|/g, '\\|');
        const _strip_slashes = (str) => str.replace(/\\([\sA-Za-z\\]|[0-7]{1,3})/g, function (str, c) {
            switch (c) {
                case "\\":
                    return "\\";
                case "0":
                    return "\u0000";
                default:
                    if (/^\w$/.test(c)) {
                        return _get_special_char(c);
                    }
                    else if (/^\s$/.test(c)) {
                        return c;
                    }
                    else if (/([0-7]{1,3})/.test(c)) {
                        return _get_ASCII_char(c);
                    }
                    return str;
            }
        });
        function _get_ASCII_char(str) {
            const num = _octal2int(str);
            return String.fromCharCode(num);
        }
        ;
        function _get_special_char(letter) {
            switch (letter.toLowerCase()) {
                case "b":
                    return "\b";
                case "f":
                    return "\f";
                case "n":
                    return "\n";
                case "r":
                    return "\r";
                case "t":
                    return "\t";
                case "v":
                    return "\v";
                default:
                    return letter;
            }
        }
        ;
        const _get_input = function (pre, next, match, type) {
            let tmp = rt.__stdinBox.text;
            let replace = `(${match})`;
            if ((type === 'STR') && (next.trim().length > 0)) {
                const before_match = _regslashs(pre);
                const after_match = _regslashs(next) + '[\\w\\W]*';
                if (before_match.length) {
                    tmp = tmp.replace(new RegExp(before_match), '');
                }
                tmp = tmp.replace(new RegExp(after_match), '');
            }
            else {
                /* C 语义：格式串里的空白匹配任意长度空白（含换行），
                 * " %c" 是"跳过空白再读一个字符"，所以前导字面量中的空白折叠为 \s* */
                const preRe = _regslashs(pre).replace(/\s+/g, '\\s*');
                replace = preRe + replace;
            }
            const m = tmp.match(new RegExp(replace));
            if (!m) {
                return null;
            }
            const result = m[1];
            if (type !== 'STR') {
                /* 非字符串模式：tmp 就是输入流本身，用匹配下标精确推进（原 indexOf 会在重复子串时错位） */
                const idx = m.index + (m[0].length - result.length);
                rt.__stdinBox.text = rt.__stdinBox.text.substr(idx + result.length);
                if (next) {
                    rt.__stdinBox.text = rt.__stdinBox.text.replace(new RegExp('^' + _regslashs(next)), '');
                }
            }
            else {
                rt.__stdinBox.text = rt.__stdinBox.text.substr(rt.__stdinBox.text.indexOf(result)).replace(result, '').replace(next, '');
            }
            return result;
        };
        /* base: 'd' 十进制 / 'u' 十进制 / 'i' 自动识别 / 'x' 十六进制 / 'o' 八进制（与 C scanf 语义一致） */
        const _get_integer = function (pre, next, base) {
            let text;
            if (base === 'i') {
                text = _get_input(pre, next, '[-+]?(?:0[xX][0-9A-Fa-f]+|0[0-7]*|[1-9][0-9]*)');
            }
            else {
                text = _get_input(pre, next, '[-+]?[0-9]+');
            }
            if (!text) {
                return null;
            }
            if (base === 'x') {
                return _hex2int(text);
            }
            if (base === 'o') {
                return _octal2int(text);
            }
            if (base === 'i') {
                if (/^[-+]?0[xX]/.test(text)) {
                    return _hex2int(text.replace(/^[-+]/, ''));
                }
                if (/^[-+]?0[0-7]+$/.test(text)) {
                    return _octal2int(text.replace(/^[-+]/, ''));
                }
                return parseInt(text, 10);
            }
            return parseInt(text, 10);
        };
        const _get_float = function (pre, next) {
            const text = _get_input(pre, next, '[-+]?(?:[0-9]+\\.?[0-9]*|\\.[0-9]+)(?:[eE][-+]?[0-9]+)?');
            if (!text) {
                return null;
            }
            return parseFloat(text);
        };
        const _get_hex = function (pre, next) {
            const text = _get_input(pre, next, '[A-Za-z0-9]+');
            return _hex2int(text);
        };
        const _get_octal = function (pre, next) {
            const text = _get_input(pre, next, '[A-Za-z0-9]+');
            return _octal2int(text);
        };
        const _get_string = function (pre, next) {
            let text = _get_input(pre, next, '([\\w\\]=-]|\\S[^\\][^\\ ])+(\\\\[\\w\\ ][\\w\\:]*)*', 'STR');
            if (/\\/.test(text)) {
                text = _strip_slashes(text);
            }
            return text;
        };
        const _get_char = function (pre, next) {
            let text = _get_input(pre, next, '.', 'STR');
            if (/\\/.test(text)) {
                text = _strip_slashes(text);
            }
            return text;
        };
        const _get_line = function (pre, next) {
            let text = _get_input(pre, next, '[^\n\r]*');
            if (/\\/.test(text)) {
                text = _strip_slashes(text);
            }
            return text;
        };
        const _deal_type = function (format) {
            const res = format.match(/%[A-Za-z]+/);
            const res2 = format.match(/[^%]*/);
            if (!res) {
                return null;
            }
            const rawType = res[0];
            let pre;
            if (!!res2) {
                pre = res2[0];
            }
            else {
                pre = null;
            }
            const next = format.substr(format.indexOf(rawType) + rawType.length);
            /* 长度修饰符归一化：%lf→%f、%lld/%ld/%hd→%d、%llu/%lu/%hu→%u、%lli→%d、%i→%d */
            let type = rawType.toLowerCase();
            if (type === '%i') {
                type = '%d';
            }
            if (/^%l{1,2}d$/.test(type) || /^%hh?d$/.test(type)) {
                type = '%d';
            }
            if (/^%l{1,2}u$/.test(type) || /^%hh?u$/.test(type)) {
                type = '%u';
            }
            if (type === '%lf') {
                type = '%f';
            }
            let ret;
            let base = null;
            switch (type) {
                case "%d":
                case "%u":
                    base = (type === '%u') ? 'u' : 'd';
                    ret = _get_integer(pre, next, base);
                    break;
                case "%c":
                    ret = _get_char(pre, next);
                    break;
                case "%s":
                    ret = _get_string(pre, next);
                    break;
                case "%S":
                    ret = _get_line(pre, next);
                    break;
                case '%x':
                case '%X':
                    ret = _get_hex(pre, next);
                    break;
                case '%o':
                case '%O':
                    ret = _get_octal(pre, next);
                    break;
                case '%f':
                    ret = _get_float(pre, next);
                    break;
                default:
                    throw new Error('Unknown type "' + rawType + '"');
            }
            return ret;
        };
        const _set_pointer_value = function (pointer, value) {
            try {
                let new_value;
                if (rt.isNormalPointerType(pointer)) {
                    if (rt.isNumericType(pointer.t.targetType)) {
                        /* %c 读入的是单字符字符串，转成字符码写入数值型目标 */
                        if (typeof value === "string") {
                            if (value.length === 0) {
                                rt.raiseException("scanf: empty char");
                            }
                            value = value.charCodeAt(0);
                        }
                        else if (value === null) {
                            return rt.raiseException("scanf: no input");
                        }
                        new_value = rt.val(pointer.t.targetType, value, true);
                        return pointer.v.target.v = new_value.v;
                    }
                    else {
                        new_value = rt.val(pointer.t.targetType, value.charCodeAt(0), true);
                        return pointer.v.target.v = new_value.v;
                    }
                }
                else if (rt.isArrayType(pointer)) {
                    const src_array = rt.makeCharArrayFromString(value);
                    if (src_array.v.target.length > pointer.v.target.length) {
                        return rt.raiseException("Not enough memory on pointer");
                    }
                    else {
                        return __range__(0, src_array.v.target.length, true).map((i) => (() => {
                            try {
                                return pointer.v.target[i] = src_array.v.target[i];
                            }
                            catch (error) {
                                return rt.raiseException("Not enough memory on pointer");
                            }
                        })());
                    }
                }
                else {
                    return rt.raiseException("Invalid Pointer Type");
                }
            }
            catch (error1) {
                return rt.raiseException("Memory overflow");
            }
        };
        const __scanf = function (format) {
            const re = new RegExp('[^%]*%[A-Za-z][^%]*', 'g');
            const selectors = format.match(re);
            return Array.from(selectors).map((val) => _deal_type(val));
        };
        const _scanf = function (rt, _this, pchar, ...args) {
            let val;
            const format = rt.getStringFromCharArray(pchar);
            const matched_values = __scanf(format);
            for (let i = 0; i < matched_values.length; i++) {
                val = matched_values[i];
                _set_pointer_value(args[i], val);
            }
            return rt.val(rt.intTypeLiteral, matched_values.length);
        };
        rt.regFunc(_scanf, "global", "scanf", [char_pointer, "?"], rt.intTypeLiteral);
        const _sscanf = function (rt, _this, original_string_pointer, format_pointer, ...args) {
            let val;
            const format = rt.getStringFromCharArray(format_pointer);
            const original_string = rt.getStringFromCharArray(original_string_pointer);
            const original_input_stream = rt.__stdinBox.text;
            rt.__stdinBox.text = original_string;
            const matched_values = __scanf(format);
            for (let i = 0; i < matched_values.length; i++) {
                val = matched_values[i];
                _set_pointer_value(args[i], val);
            }
            rt.__stdinBox.text = original_input_stream;
            return rt.val(rt.intTypeLiteral, matched_values.length);
        };
        return rt.regFunc(_sscanf, "global", "sscanf", [char_pointer, char_pointer, "?"], rt.intTypeLiteral);
    }
};
//# sourceMappingURL=cstdio.js.map
