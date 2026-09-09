"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const rt_1 = require("./rt");
const interpreter_1 = require("./interpreter");
const ast = require("./ast");
const preprocessor = require("./preprocessor");
const debugger_1 = require("./debugger");
const PEGUtil = require("pegjs-util");
const includes = {
    iostream: require("./includes/iostream"),
    cctype: require("./includes/cctype"),
    cstring: require("./includes/cstring"),
    cmath: require("./includes/cmath"),
    cstdio: require("./includes/cstdio"),
    cstdlib: require("./includes/cstdlib"),
    ctime: require("./includes/ctime"),
    iomanip: require("./includes/iomanip"),
    foo: require("./includes/dummy_class_foo")
};
const headerAlias = {
    "ctype.h": "cctype",
    "string.h": "cstring",
    "math.h": "cmath",
    "stdio.h": "cstdio",
    "stdlib.h": "cstdlib",
    "time.h": "ctime"
};
for (const alias of Object.keys(headerAlias)) {
    const realName = headerAlias[alias];
    includes[alias] = includes[realName];
}
function run(code, input, config) {
    let step;
    let inputbuffer = input.toString();
    let drainedCache = null;
    const _config = {
        stdio: {
            drain() {
                if (drainedCache === null) {
                    drainedCache = inputbuffer;
                    inputbuffer = null;
                }
                return drainedCache;
            },
            write(s) {
                process.stdout.write(s);
            }
        },
        includes: this.includes,
        unsigned_overflow: "error"
    };
    rt_1.mergeConfig(_config, config);
    const rt = new rt_1.CRuntime(_config);
    code = code.toString();
    const oldCode = code;
    const __lits = [];
    code = code.replace(/"(?:[^"\\\n]|\\.)*"|'(?:[^'\\\n]|\\.)*'|\/\*[\s\S]*?\*\/|\/\/[^\n]*/g, function (m) {
        if (m.charAt(0) === '"' || m.charAt(0) === "'") {
            __lits.push(m);
            return m.charAt(0) + "\u0000" + (__lits.length - 1) + "\u0000" + m.charAt(0);
        }
        return m.replace(/[^\n]/g, " ");
    });
    code = preprocessor.parse(rt, code);
    code = code.replace(/(["'])\u0000(\d+)\u0000\1/g, function (m, q, i) { return __lits[+i]; });
    const mydebugger = new debugger_1.default(code, oldCode);
    const result = PEGUtil.parse(ast, code);
    if (result.error != null) {
        throw new Error("ERROR: Parsing Failure:\n" + PEGUtil.errorMessage(result.error, true));
    }
    const interpreter = new interpreter_1.Interpreter(rt);
    const defGen = interpreter.run(result.ast, code);
    while (true) {
        step = defGen.next();
        if (step.done) {
            break;
        }
    }
    const mainGen = rt.getFunc("global", "main", [])(rt, null);
    if (_config.debug) {
        mydebugger.start(rt, mainGen);
        return mydebugger;
    }
    else {
        const startTime = Date.now();
        while (true) {
            step = mainGen.next();
            if (step.done) {
                break;
            }
            if (_config.maxTimeout && ((Date.now() - startTime) > _config.maxTimeout)) {
                throw new Error("Time limit exceeded.");
            }
        }
        return step.value.v;
    }
}
exports.default = {
    includes,
    run,
};
//# sourceMappingURL=launcher.js.map