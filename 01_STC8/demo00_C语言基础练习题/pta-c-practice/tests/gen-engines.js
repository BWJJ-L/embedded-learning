"use strict";
/* 已废弃：JSCPP 引擎构建请用 build-engine.js（含解释器修补），Pyodide 离线引擎请用 build-pyodide.js。
 * 本文件保留仅作历史参考；运行它会用未修补的原始引擎覆盖 vendor，请勿再执行。 */
if (!process.env.FORCE_GEN_ENGINES) {
    console.log("此脚本已废弃，请改用：node build-engine.js / node build-pyodide.js");
    process.exit(0);
}
/* 生成引擎内嵌文件（一次性构建脚本）：
 *   vendor/jscpp.engine.js  → window.__ENGINE_SRC_JSCPP = "<JSCPP 完整源码字符串>"
 *   vendor/pyodide.engine.js → window.__ENGINE_SRC_PYODIDE = "<Pyodide 加载器源码字符串>"
 * 页面用普通 <script> 标签加载它们（file:// 下不经过 fetch，无 CORS 问题），
 * 判题器再把字符串内联进 Blob Worker。
 * 重新生成：node tests/gen-engines.js
 */
const fs = require("fs");
const path = require("path");

const vendor = path.join(__dirname, "..", "vendor");

function embed(outName, srcFile, globalName, note) {
    const src = fs.readFileSync(path.join(vendor, srcFile), "utf8");
    const out = "/* " + note + "（由 tests/gen-engines.js 生成，请勿手改） */\n" +
        "window." + globalName + " = " + JSON.stringify(src) + ";\n";
    fs.writeFileSync(path.join(vendor, outName), out, "utf8");
    console.log(outName, "→", out.length, "bytes（源文件", src.length, "bytes）");
}

embed("jscpp.engine.js", "jscpp.es5.min.js", "__ENGINE_SRC_JSCPP", "JSCPP 解释器源码（C/C++ 判题引擎）");
embed("pyodide.engine.js", "pyodide.js", "__ENGINE_SRC_PYODIDE", "Pyodide 加载器源码（Python 判题引擎，wasm 仍需 CDN 或缓存）");
console.log("完成");
