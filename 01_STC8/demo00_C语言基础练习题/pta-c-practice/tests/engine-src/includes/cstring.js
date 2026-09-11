"use strict";
/* 本文件由 tests/build-engine.js 覆盖到 engine-src（基于 JSCPP 2.0.9 lib/includes/cstring.js 修补）：
 * 1. 修复原实现的致命 bug：rt.isArrayType(v) 少传 .t，导致 strlen/strcmp/strncmp/strchr/strrchr/strstr 全部
 *    抛 "not an array"（strncat 因内部调用 strncpy 同样失败）
 * 2. strcmp 比较改用各自的 position 并归一化为 -1/0/1
 * 3. strstr 的第二参数注册为 char*（原来注册成 char，调用必失败），匹配逻辑重写
 * 4. 新增 strtok / memset / memcpy / memmove / memcmp（memset/memcpy 按元素大小折算字节数，
 *    支持 memset(int数组, 0, sizeof(数组)) 这类教学常用写法）
 * 5. 注册 NULL 为空指针常量（strtok 循环里要用）
 */
module.exports = {
    load(rt) {
        const pchar = rt.normalPointerType(rt.charTypeLiteral);
        const sizet = rt.primitiveType("unsigned int");
        rt.scope[0].variables["NULL"] = rt.val(pchar, rt.nullPointerValue);

        /* 统一取出底层字符格点数组与起始下标：兼容数组值与（数组/普通）指针值 */
        function _bufOf(rt, v) {
            if (v && v.v && Array.isArray(v.v.target)) {
                return { a: v.v.target, p: v.v.position || 0 };
            }
            if (v && v.v && v.v.target && typeof v.v.target.v !== "undefined") {
                return { a: [v.v.target], p: 0 };
            }
            rt.raiseException("argument is not a char pointer");
        }
        function _strlenCells(rt, v) {
            const { a, p } = _bufOf(rt, v);
            let i = p;
            while (i < a.length && a[i].v !== 0) {
                i++;
            }
            if (i >= a.length && a.length > 0 && a[a.length - 1].v !== 0) {
                /* 数组填满仍未见 \0：按 C 语义返回到数组末尾的长度 */
            }
            return i - p;
        }
        const _strcpy = require("./shared/cstring_strcpy");
        rt.regFunc(_strcpy, "global", "strcpy", [pchar, pchar], pchar);

        rt.regFunc(function (rt, _this, dest, src, num) {
            const d = _bufOf(rt, dest);
            const s = _bufOf(rt, src);
            let n = num.v;
            let i = s.p;
            let j = d.p;
            /* 拷贝至多 n 个字符；源不足 n 时按 C 语义补 \0 到 n 个 */
            while (n > 0 && i < s.a.length && s.a[i].v !== 0) {
                if (j >= d.a.length) {
                    rt.raiseException("destination array is not big enough");
                }
                d.a[j] = rt.clone(s.a[i]);
                j++; i++; n--;
            }
            if (n > 0) {
                while (n > 0 && j < d.a.length) {
                    d.a[j] = rt.val(rt.charTypeLiteral, 0);
                    j++; n--;
                }
            }
            else if (j < d.a.length && i < s.a.length && s.a[i].v !== 0) {
                d.a[j] = rt.val(rt.charTypeLiteral, 0);
            }
            return dest;
        }, "global", "strncpy", [pchar, pchar, sizet], pchar);

        rt.regFunc(function (rt, _this, dest, src) {
            const lendest = _strlenCells(rt, dest);
            const d = _bufOf(rt, dest);
            const s = _bufOf(rt, src);
            let j = d.p + lendest;
            let i = s.p;
            while (i < s.a.length && s.a[i].v !== 0) {
                if (j >= d.a.length - 1) {
                    rt.raiseException("destination array is not big enough");
                }
                d.a[j] = rt.clone(s.a[i]);
                j++; i++;
            }
            if (j < d.a.length) {
                d.a[j] = rt.val(rt.charTypeLiteral, 0);
            }
            return dest;
        }, "global", "strcat", [pchar, pchar], pchar);

        rt.regFunc(function (rt, _this, dest, src, num) {
            const lendest = _strlenCells(rt, dest);
            const d = _bufOf(rt, dest);
            const s = _bufOf(rt, src);
            let n = num.v;
            let j = d.p + lendest;
            let i = s.p;
            while (n > 0 && i < s.a.length && s.a[i].v !== 0) {
                if (j >= d.a.length - 1) {
                    rt.raiseException("destination array is not big enough");
                }
                d.a[j] = rt.clone(s.a[i]);
                j++; i++; n--;
            }
            if (j < d.a.length) {
                d.a[j] = rt.val(rt.charTypeLiteral, 0);
            }
            return dest;
        }, "global", "strncat", [pchar, pchar, sizet], pchar);

        rt.regFunc(function (rt, _this, str) {
            return rt.val(sizet, _strlenCells(rt, str));
        }, "global", "strlen", [pchar], sizet);

        rt.regFunc(function (rt, _this, s1, s2) {
            const x = _bufOf(rt, s1);
            const y = _bufOf(rt, s2);
            let i = x.p, j = y.p;
            while (i < x.a.length && j < y.a.length && x.a[i].v !== 0 && y.a[j].v !== 0 && x.a[i].v === y.a[j].v) {
                i++; j++;
            }
            const ca = (i < x.a.length) ? x.a[i].v : 0;
            const cb = (j < y.a.length) ? y.a[j].v : 0;
            const d = ca - cb;
            return rt.val(rt.intTypeLiteral, d < 0 ? -1 : (d > 0 ? 1 : 0));
        }, "global", "strcmp", [pchar, pchar], rt.intTypeLiteral);

        rt.regFunc(function (rt, _this, s1, s2, num) {
            const x = _bufOf(rt, s1);
            const y = _bufOf(rt, s2);
            let n = num.v;
            let i = x.p, j = y.p;
            while (n > 0 && i < x.a.length && j < y.a.length && x.a[i].v !== 0 && y.a[j].v !== 0 && x.a[i].v === y.a[j].v) {
                i++; j++; n--;
            }
            if (n === 0) {
                return rt.val(rt.intTypeLiteral, 0);
            }
            const ca = (i < x.a.length) ? x.a[i].v : 0;
            const cb = (j < y.a.length) ? y.a[j].v : 0;
            const d = ca - cb;
            return rt.val(rt.intTypeLiteral, d < 0 ? -1 : (d > 0 ? 1 : 0));
        }, "global", "strncmp", [pchar, pchar, sizet], rt.intTypeLiteral);

        /* strchr/strrchr/strstr 返回带数组类型的指针值：JSCPP 的 isStringType 只认数组类型，
         * 这样 printf("%s", p) 与 p-s 等用法都和 s+i 的行为一致 */
        rt.regFunc(function (rt, _this, str, ch) {
            const { a, p } = _bufOf(rt, str);
            let i = p;
            while (i < a.length && a[i].v !== 0 && a[i].v !== ch.v) {
                i++;
            }
            const arrType = rt.arrayPointerType(rt.charTypeLiteral, a.length);
            if (i < a.length && a[i].v === ch.v) {
                return rt.val(arrType, rt.makeArrayPointerValue(a, i));
            }
            return rt.val(arrType, rt.nullPointerValue);
        }, "global", "strchr", [pchar, rt.charTypeLiteral], pchar);

        rt.regFunc(function (rt, _this, str, ch) {
            const { a, p } = _bufOf(rt, str);
            let i = p;
            let lastpos = -1;
            while (i < a.length && a[i].v !== 0) {
                if (a[i].v === ch.v) {
                    lastpos = i;
                }
                i++;
            }
            const arrType = rt.arrayPointerType(rt.charTypeLiteral, a.length);
            if (lastpos >= 0) {
                return rt.val(arrType, rt.makeArrayPointerValue(a, lastpos));
            }
            return rt.val(arrType, rt.nullPointerValue);
        }, "global", "strrchr", [pchar, rt.charTypeLiteral], pchar);

        rt.regFunc(function (rt, _this, str1, str2) {
            const x = _bufOf(rt, str1);
            const y = _bufOf(rt, str2);
            const arrType = rt.arrayPointerType(rt.charTypeLiteral, x.a.length);
            let i = x.p;
            while (i < x.a.length && x.a[i].v !== 0) {
                let j = y.p;
                let k = i;
                while (k < x.a.length && j < y.a.length && y.a[j].v !== 0 && x.a[k].v === y.a[j].v) {
                    k++; j++;
                }
                if (j < y.a.length && y.a[j].v === 0) {
                    return rt.val(arrType, rt.makeArrayPointerValue(x.a, i));
                }
                i++;
            }
            return rt.val(arrType, rt.nullPointerValue);
        }, "global", "strstr", [pchar, pchar], pchar);

        /* strtok：状态保存在 rt 上（与 C 的跨调用静态状态一致） */
        rt.regFunc(function (rt, _this, str, delim) {
            const d = _bufOf(rt, delim);
            let s;
            let nullType = pchar;
            if (str === null || str === undefined || str.v.target === rt.nullPointerValue || str.v.target === null) {
                s = rt.__strtokState;
                if (!s) {
                    return rt.val(pchar, rt.nullPointerValue);
                }
            }
            else {
                const b = _bufOf(rt, str);
                s = { a: b.a, p: b.p };
            }
            /* 跳过前导分隔符 */
            let i = s.p;
            while (i < s.a.length && s.a[i].v !== 0 && d.a.some(function (c) { return c.v === s.a[i].v; })) {
                i++;
            }
            const arrType = rt.arrayPointerType(rt.charTypeLiteral, s.a.length);
            if (i >= s.a.length || s.a[i].v === 0) {
                rt.__strtokState = null;
                return rt.val(arrType, rt.nullPointerValue);
            }
            const start = i;
            while (i < s.a.length && s.a[i].v !== 0 && !d.a.some(function (c) { return c.v === s.a[i].v; })) {
                i++;
            }
            let next;
            if (i < s.a.length && s.a[i].v !== 0) {
                s.a[i] = rt.val(rt.charTypeLiteral, 0);
                next = { a: s.a, p: i + 1 };
            }
            else {
                next = null;
            }
            rt.__strtokState = next;
            return rt.val(arrType, rt.makeArrayPointerValue(s.a, start));
        }, "global", "strtok", [pchar, pchar], pchar);

        /* memset / memcpy / memmove / memcmp：单一 "?" 可变参注册（按元素类型注册多个重载会
         * 触发 "ambiguous method invoking"）；数量参数按 C 语义解释为字节，内部折算成元素个数 */
        function _bytesOf(rt, t) {
            const lim = rt.config.limits[t.name];
            return (lim && lim.bytes) ? lim.bytes : 1;
        }
        function _targetOf(rt, v) {
            /* 兼容数组与指针，返回 { cells, start, eleType } */
            if (rt.isArrayType(v.t)) {
                return { cells: v.v.target, start: v.v.position || 0, eleType: v.t.eleType };
            }
            if (rt.isNormalPointerType(v.t)) {
                if (Array.isArray(v.v.target)) {
                    return { cells: v.v.target, start: v.v.position || 0, eleType: v.t.targetType };
                }
                if (v.v.target && typeof v.v.target.v !== "undefined") {
                    return { cells: [v.v.target], start: 0, eleType: v.t.targetType };
                }
            }
            rt.raiseException("mem function: argument is not a pointer");
        }
        rt.regFunc(function (rt, _this, ...args) {
            if (args.length !== 3) rt.raiseException("memset(dest, value, count) 需要 3 个参数");
            const t = _targetOf(rt, args[0]);
            const per = _bytesOf(rt, t.eleType);
            const count = Math.floor(args[2].v / per);
            const v = rt.cast(t.eleType, args[1]).v;
            for (let i = 0; i < count && t.start + i < t.cells.length; i++) {
                t.cells[t.start + i] = rt.val(t.eleType, v);
            }
            return args[0];
        }, "global", "memset", ["?"], pchar);
        rt.regFunc(function (rt, _this, ...args) {
            if (args.length !== 3) rt.raiseException("memcpy(dest, src, count) 需要 3 个参数");
            const d = _targetOf(rt, args[0]);
            const s = _targetOf(rt, args[1]);
            const count = Math.floor(args[2].v / _bytesOf(rt, d.eleType));
            for (let i = 0; i < count && d.start + i < d.cells.length; i++) {
                const sc = s.cells[s.start + i];
                d.cells[d.start + i] = sc ? rt.clone(sc) : rt.val(d.eleType, 0);
            }
            return args[0];
        }, "global", "memcpy", ["?"], pchar);
        rt.regFunc(function (rt, _this, ...args) {
            if (args.length !== 3) rt.raiseException("memmove(dest, src, count) 需要 3 个参数");
            const d = _targetOf(rt, args[0]);
            const s = _targetOf(rt, args[1]);
            const count = Math.floor(args[2].v / _bytesOf(rt, d.eleType));
            const tmp = [];
            for (let i = 0; i < count; i++) {
                const sc = s.cells[s.start + i];
                tmp.push(sc ? rt.clone(sc) : rt.val(d.eleType, 0));
            }
            for (let i = 0; i < count && d.start + i < d.cells.length; i++) {
                d.cells[d.start + i] = tmp[i];
            }
            return args[0];
        }, "global", "memmove", ["?"], pchar);
        rt.regFunc(function (rt, _this, ...args) {
            if (args.length !== 3) rt.raiseException("memcmp(s1, s2, count) 需要 3 个参数");
            const a = _targetOf(rt, args[0]);
            const b = _targetOf(rt, args[1]);
            const count = Math.floor(args[2].v / _bytesOf(rt, a.eleType));
            for (let i = 0; i < count; i++) {
                const ca = a.cells[a.start + i] ? a.cells[a.start + i].v : 0;
                const cb = b.cells[b.start + i] ? b.cells[b.start + i].v : 0;
                if (ca !== cb) {
                    return rt.val(rt.intTypeLiteral, ca < cb ? -1 : 1);
                }
            }
            return rt.val(rt.intTypeLiteral, 0);
        }, "global", "memcmp", ["?"], pchar);
    }
};
//# sourceMappingURL=cstring.js.map
