"use strict";
/* 下载 Pyodide v0.26.4 发行文件到 tests/pyodide-dist/（一次性，构建 vendor/pyodide.engine.js 用） */
const https = require("https");
const fs = require("fs");
const path = require("path");

const BASE = "https://cdn.jsdelivr.net/pyodide/v0.26.4/full/";
const FILES = ["pyodide.js", "pyodide.mjs", "pyodide.asm.js", "pyodide.asm.wasm", "python_stdlib.zip", "pyodide-lock.json"];
const OUT = path.join(__dirname, "pyodide-dist");

fs.mkdirSync(OUT, { recursive: true });

function get(url, dest) {
    return new Promise((resolve, reject) => {
        const req = https.get(url, (res) => {
            if (res.statusCode !== 200) { res.resume(); return reject(new Error(url + " → HTTP " + res.statusCode)); }
            const f = fs.createWriteStream(dest);
            res.pipe(f);
            f.on("finish", () => f.close(() => resolve(dest)));
            f.on("error", reject);
        });
        req.on("error", reject);
    });
}

(async () => {
    for (const name of FILES) {
        const dest = path.join(OUT, name);
        if (fs.existsSync(dest) && fs.statSync(dest).size > 1000) {
            console.log("已存在，跳过:", name, fs.statSync(dest).size, "bytes");
            continue;
        }
        process.stdout.write("下载 " + name + " … ");
        await get(BASE + name, dest);
        console.log(fs.statSync(dest).size, "bytes");
    }
    console.log("全部就绪");
})().catch((e) => { console.error("失败:", e.message); process.exit(1); });
