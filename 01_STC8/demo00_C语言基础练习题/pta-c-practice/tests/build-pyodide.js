"use strict";
/* ============================================================
 * Pyodide 离线引擎构建脚本
 *
 * 产物 vendor/pyodide.engine.js（约 18MB）：
 *   window.__ENGINE_SRC_PYODIDE = "<Worker 自举源码字符串>"
 * 自举源码包含：
 *   1. fetch 垫片：对 pyodide.asm.wasm / python_stdlib.zip / pyodide-lock.json
 *      直接用内嵌的 base64/文本构造 Response（默认阻止其它网络请求，保证零联网）；
 *   2. eval 内嵌的 pyodide.asm.js（预定义 _createPyodideModule，
 *      loadPyodide 检测到后跳过 importScripts 下载）；
 *   3. eval 内嵌的 pyodide.js 包装器（提供 loadPyodide）。
 * 之后 judge.js 现有的 loadPyodide({indexURL}) 流程原样工作，全部资源由垫片供给。
 *
 * 重新生成：先 node tests/fetch-pyodide.js 下载发行文件，再 node tests/build-pyodide.js
 * ============================================================ */
const fs = require("fs");
const path = require("path");

const DIST = path.join(__dirname, "pyodide-dist");
const VENDOR = path.join(__dirname, "..", "vendor");

function mustRead(name) {
    const p = path.join(DIST, name);
    if (!fs.existsSync(p)) {
        throw new Error("缺少 " + p + "，请先运行 node tests/fetch-pyodide.js");
    }
    return fs.readFileSync(p);
}
function b64(name) { return mustRead(name).toString("base64"); }
function text(name) { return mustRead(name).toString("utf8"); }

/* 预检：确保加载器源码里的三类网络请求都已被垫片覆盖 */
const asmjs = text("pyodide.asm.js");
const wrapper = text("pyodide.js");
for (const marker of ["pyodide.asm.wasm", "python_stdlib.zip", "pyodide-lock.json"]) {
    if (wrapper.indexOf(marker) < 0) {
        throw new Error("pyodide.js 中未找到对 " + marker + " 的引用，垫片清单需要复核");
    }
}

const boot = [
    "/* Pyodide v0.26.4 离线自举（由 tests/build-pyodide.js 生成，请勿手改）",
    " * fetch 垫片 + 内嵌 wasm/标准库：默认拦截一切非内嵌资源的网络请求，完全离线可用。",
    " * 需要恢复在线行为时，把 __OFFLINE_ONLY 改为 false。 */",
    "var __OFFLINE_ONLY = true;",
    "var __ASSETS = {",
    '  "pyodide.asm.wasm": { b64: ' + JSON.stringify(b64("pyodide.asm.wasm")) + ', type: "application/wasm" },',
    '  "python_stdlib.zip": { b64: ' + JSON.stringify(b64("python_stdlib.zip")) + ', type: "application/zip" },',
    '  "pyodide-lock.json": { text: ' + JSON.stringify(text("pyodide-lock.json")) + ', type: "application/json" },',
    '  "pyodide.asm.js": { text: ' + JSON.stringify(asmjs) + ', type: "text/javascript" },',
    '  "pyodide.js": { text: ' + JSON.stringify(wrapper) + ', type: "text/javascript" }',
    "};",
    "var __REAL_FETCH = self.fetch.bind(self);",
    "var __decoded = {};",
    "function __b64ToBytes(b64) {",
    "  var bin = atob(b64);",
    "  var len = bin.length;",
    "  var bytes = new Uint8Array(len);",
    "  for (var i = 0; i < len; i++) bytes[i] = bin.charCodeAt(i);",
    "  return bytes;",
    "}",
    "function __assetNameOf(url) {",
    "  var m = /\\/([^\\/?#]+)(?:[?#].*)?$/.exec(String(url));",
    "  var name = m ? m[1] : '';",
    "  return __ASSETS.hasOwnProperty(name) ? name : null;",
    "}",
    "self.fetch = function (input, init) {",
    "  var url = (input && typeof input === 'object' && input.url) ? input.url : String(input);",
    "  var name = __assetNameOf(url);",
    "  if (name) {",
    "    var a = __ASSETS[name];",
    "    if (!__decoded[name]) { __decoded[name] = 1; }",
    "    if (a.b64) {",
    "      if (!a._bytes) { a._bytes = __b64ToBytes(a.b64); }",
    "      return Promise.resolve(new Response(a._bytes, { status: 200, headers: { 'Content-Type': a.type } }));",
    "    }",
    "    return Promise.resolve(new Response(a.text, { status: 200, headers: { 'Content-Type': a.type } }));",
    "  }",
    "  if (__OFFLINE_ONLY) {",
    "    return Promise.reject(new Error('离线模式：Python 运行时已完整内置，无需访问 ' + url));",
    "  }",
    "  return __REAL_FETCH(input, init);",
    "};",
    "/* 预先 eval pyodide.asm.js 定义 _createPyodideModule，loadPyodide 检测到后跳过 importScripts */",
    "self.eval(__ASSETS['pyodide.asm.js'].text);",
    "self.eval(__ASSETS['pyodide.js'].text);"
].join("\n");

const out = "/* Pyodide 离线引擎（由 tests/build-pyodide.js 生成，请勿手改；内嵌 wasm 与标准库，约 18MB） */\n" +
    "window.__ENGINE_SRC_PYODIDE = " + JSON.stringify(boot) + ";\n";
fs.writeFileSync(path.join(VENDOR, "pyodide.engine.js"), out, "utf8");
console.log("vendor/pyodide.engine.js:", (out.length / 1024 / 1024).toFixed(1) + " MB");
console.log("完成 ✓（Python 判题现已完全离线）");
