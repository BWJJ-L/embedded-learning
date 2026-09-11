"use strict";
/* ============================================================
 * JSCPP 引擎补丁构建脚本（修复 README「已知限制」中的解释器缺陷）
 *
 * 流程：
 *   1. tests/engine-orig/   = JSCPP 2.0.9 原始 lib（pristine 基线，勿手改）
 *   2. 复制 → tests/engine-src/，叠加 tests/engine-patches/ 整文件补丁
 *      （includes/cstdio.js、includes/cstring.js、includes/iostream.js）
 *   3. 对 launcher.js / defaults.js / rt.js 做手术级正则补丁（见下 PATCHES）
 *   4. esbuild 打包 engine-src/entry.js → vendor/jscpp.es5.min.js
 *   5. 重新生成 vendor/jscpp.engine.js（内嵌源码字符串）
 *   6. verify.js / jscpp-limits.js 优先加载 tests/engine-src（补丁版）
 *
 * 重跑：node tests/build-engine.js（幂等；npm install 重装后同样先跑本脚本）
 * ============================================================ */
const fs = require("fs");
const path = require("path");
const { execSync } = require("child_process");

const testsDir = __dirname;
const projectDir = path.join(testsDir, "..");
const vendorDir = path.join(projectDir, "vendor");
const ORIG = path.join(testsDir, "engine-orig");
const SRC = path.join(testsDir, "engine-src");
const PATCHES = path.join(testsDir, "engine-patches");

/* 注意：本机 Node 22.23.2 的 fs.cpSync 原生绑定在含中文的路径上会静默崩溃（exit 127），
 * 因此这里一律用 readdir/copyFile/mkdir/unlink/rmdir 手写复制与删除 */
function copyTree(src, dest) {
    fs.mkdirSync(dest, { recursive: true });
    for (const name of fs.readdirSync(src)) {
        const s = path.join(src, name);
        const d = path.join(dest, name);
        if (fs.statSync(s).isDirectory()) copyTree(s, d);
        else fs.copyFileSync(s, d);
    }
}
function removeTree(p) {
    if (!fs.existsSync(p)) return;
    for (const name of fs.readdirSync(p)) {
        const full = path.join(p, name);
        if (fs.statSync(full).isDirectory()) removeTree(full);
        else fs.unlinkSync(full);
    }
    fs.rmdirSync(p);
}

/* ---------- 0. 基线：没有 engine-orig 就从 node_modules 拷一份 ---------- */
if (!fs.existsSync(path.join(ORIG, "commonjs.js"))) {
    copyTree(path.join(testsDir, "node_modules", "JSCPP", "lib"), ORIG);
    console.log("engine-orig 基线已从 node_modules/JSCPP/lib 建立");
}

/* ---------- 1. 干净复制 orig → src ---------- */
removeTree(SRC);
copyTree(ORIG, SRC);

/* ---------- 2. 叠加整文件补丁 ---------- */
function walk(dir, cb) {
    for (const name of fs.readdirSync(dir)) {
        const p = path.join(dir, name);
        const st = fs.statSync(p);
        if (st.isDirectory()) walk(p, cb);
        else cb(p);
    }
}
let overlayCount = 0;
walk(PATCHES, (p) => {
    const rel = path.relative(PATCHES, p);
    const dest = path.join(SRC, rel);
    fs.mkdirSync(path.dirname(dest), { recursive: true });
    fs.copyFileSync(p, dest);
    overlayCount++;
});
console.log("整文件补丁叠加:", overlayCount, "个");

/* ---------- 3. 手术级补丁（逐条断言命中，未命中即报错） ---------- */
function patchFile(rel, replacements) {
    const p = path.join(SRC, rel);
    let src = fs.readFileSync(p, "utf8");
    const isCRLF = src.includes("\r\n");
    for (const [oldStr, newStr] of replacements) {
        let o = isCRLF ? oldStr.replace(/\n/g, "\r\n") : oldStr;
        let n = isCRLF ? newStr.replace(/\n/g, "\r\n") : newStr;
        if (!src.includes(o)) {
            throw new Error(`补丁未命中 [${rel}]: ${oldStr.slice(0, 60)}...`);
        }
        src = src.replace(o, n);
    }
    fs.writeFileSync(p, src, "utf8");
    console.log("手术补丁:", rel, `(${replacements.length} 处)`);
}

patchFile("launcher.js", [
    /* 多个头文件（cstdio 与 iostream 混用）都会 drain 输入，缓存后返回同一份完整输入，
       修复第二次 drain 拿到 null 导致的解释器崩溃 */
    [
        `drain() {\n                const x = inputbuffer;\n                inputbuffer = null;\n                return x;\n            },`,
        `drain() {\n                if (drainedCache === null) {\n                    drainedCache = inputbuffer;\n                    inputbuffer = null;\n                }\n                return drainedCache;\n            },`
    ],
    [
        `    const _config = {`,
        `    let drainedCache = null;\n    const _config = {`
    ],
    /* 预处理器把字符串字面量按宏参数切分（"a, b" 会变成 "a,b"）：
       先把注释抹成等长空白、字符串/字符字面量掩蔽为占位符，预处理完成后还原 */
    [
        `    const oldCode = code;\n    code = preprocessor.parse(rt, code);`,
        `    const oldCode = code;\n    const __lits = [];\n    code = code.replace(/"(?:[^"\\\\\\n]|\\\\.)*"|'(?:[^'\\\\\\n]|\\\\.)*'|\\/\\*[\\s\\S]*?\\*\\/|\\/\\/[^\\n]*/g, function (m) {\n        if (m.charAt(0) === '"' || m.charAt(0) === "'") {\n            __lits.push(m);\n            return m.charAt(0) + "\\u0000" + (__lits.length - 1) + "\\u0000" + m.charAt(0);\n        }\n        return m.replace(/[^\\n]/g, " ");\n    });\n    code = preprocessor.parse(rt, code);\n    code = code.replace(/(["'])\\u0000(\\d+)\\u0000\\1/g, function (m, q, i) { return __lits[+i]; });`
    ]
]);

patchFile("defaults.js", [
    /* 负数整数除法按 C 语义向零取整（原 Math.floor 向负无穷，-7/2 得 -4） */
    [
        `                    if (rt.isIntegerType(l.t) && rt.isIntegerType(r.t)) {\n                        ret = Math.floor(ret);\n                    }`,
        `                    if (rt.isIntegerType(l.t) && rt.isIntegerType(r.t)) {\n                        ret = Math.trunc(ret);\n                    }`
    ]
]);

patchFile("rt.js", [
    /* 未初始化变量默认值 NaN→0：NaN 会让后续任何读取触发 "overflow of NaN"，
       现与真实 C 常见表现一致（新栈上为 0），未初始化不再是报错点 */
    [
        `defaultValue(type, left = false) {\n        if (type.type === "primitive") {\n            return this.val(type, NaN, left, true);`,
        `defaultValue(type, left = false) {\n        if (type.type === "primitive") {\n            return this.val(type, 0, left, true);`
    ],
    /* getStringFromCharArray 尊重指针偏移：printf("%s", p+2) / puts(p+1) 原来从头输出 */
    [
        `        if (this.isStringType(element.t)) {\n            const { target } = element.v;\n            let result = "";\n            let i = 0;`,
        `        if (this.isStringType(element.t)) {\n            const { target } = element.v;\n            let result = "";\n            let i = element.v.position || 0;`
    ],
    /* 指针→bool：while(p)、if(p) 这类指针真值判断（strtok 循环等）原来直接抛 cast failed */
    [
        `        if (this.isTypeEqualTo(value.t, type)) {\n            return value;\n        }\n        if (this.isPrimitiveType(type) && this.isPrimitiveType(value.t)) {`,
        `        if (this.isTypeEqualTo(value.t, type)) {\n            return value;\n        }\n        if (type.type === "primitive" && type.name === "bool" && value.t && this.isPointerType(value.t)) {\n            const __tgt = value.v.target;\n            const __isNull = (__tgt === null || __tgt === this.nullPointerValue);\n            return this.val(type, !__isNull);\n        }\n        if (this.isPrimitiveType(type) && this.isPrimitiveType(value.t)) {`
    ]
]);

patchFile("interpreter.js", [
    /* (char*)p 强转：原 TypeName 实现忽略指针部分，(char*)p 会按 char 处理报 cast failed */
    [
        `            TypeName(interp, s, param) {\n                ({\n                    rt\n                } = interp);\n                const typename = [];\n                for (const baseType of s.base) {\n                    if (baseType !== "const") {\n                        typename.push(baseType);\n                    }\n                }\n                return rt.simpleType(typename);\n            },`,
        `            TypeName(interp, s, param) {\n                ({\n                    rt\n                } = interp);\n                const typename = [];\n                for (const baseType of s.base) {\n                    if (baseType !== "const") {\n                        typename.push(baseType);\n                    }\n                }\n                let __t = rt.simpleType(typename);\n                if (s.extra && s.extra.type === "AbstractDeclarator" && s.extra.Pointer && s.extra.Pointer.length > 0) {\n                    __t = interp.buildRecursivePointerType(s.extra.Pointer, __t, 0);\n                }\n                return __t;\n            },`
    ]
]);

/* ---------- 4. esbuild 打包 ---------- */
fs.writeFileSync(path.join(SRC, "entry.js"),
    `const JSCPP = require("./commonjs.js");\nwindow.JSCPP = JSCPP;\n`, "utf8");
/* printf 包在浏览器用不到的 Stream 分支 require('stream')，util.inspect 也只用于对象调试输出，
 * 都用空壳替代 */
fs.writeFileSync(path.join(SRC, "stub-stream.js"),
    `module.exports = { Stream: function Stream() {} };\n`, "utf8");
fs.writeFileSync(path.join(SRC, "stub-util.js"),
    `module.exports = { inspect: function (x) { return typeof x === "string" ? x : String(x); } };\n`, "utf8");

let esbuild;
try {
    esbuild = require("esbuild");
} catch (e) {
    console.log("安装 esbuild…");
    execSync("npm install --no-audit --no-fund --save-dev esbuild", { cwd: testsDir, stdio: "inherit" });
    esbuild = require("esbuild");
}
const bundle = esbuild.buildSync({
    entryPoints: [path.join(SRC, "entry.js")],
    bundle: true,
    minify: true,
    format: "iife",
    platform: "browser",
    target: "es2018",
    alias: { stream: path.join(SRC, "stub-stream.js"), util: path.join(SRC, "stub-util.js") },
    write: false
});
const outCode = bundle.outputFiles[0].text;
fs.writeFileSync(path.join(vendorDir, "jscpp.es5.min.js"), outCode, "utf8");
console.log("vendor/jscpp.es5.min.js:", outCode.length, "bytes（打补丁后的浏览器 bundle）");

/* ---------- 5. 重生成 vendor/jscpp.engine.js（内嵌字符串） ---------- */
const embedded = "/* JSCPP 解释器源码（C/C++ 判题引擎，由 tests/build-engine.js 生成，请勿手改） */\n" +
    "window.__ENGINE_SRC_JSCPP = " + JSON.stringify(outCode) + ";\n";
fs.writeFileSync(path.join(vendorDir, "jscpp.engine.js"), embedded, "utf8");
console.log("vendor/jscpp.engine.js:", embedded.length, "bytes");
console.log("完成 ✓");
