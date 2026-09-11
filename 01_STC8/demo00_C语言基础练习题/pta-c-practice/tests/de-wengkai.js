"use strict";
/* 清理全部文件中的“翁恺”字样 + 更新 README 中的旧题号表述 */
const fs = require("fs");
const path = require("path");
const root = path.join(__dirname, "..");

function fix(p, pairs) {
    let s = fs.readFileSync(p, "utf8");
    let n = 0;
    pairs.forEach(function (pr) {
        if (s.indexOf(pr[0]) !== -1) { s = s.split(pr[0]).join(pr[1]); n++; }
    });
    fs.writeFileSync(p, s, "utf8");
    console.log(path.relative(root, p), "替换", n, "组");
}

fix(path.join(root, "index.html"), [
    ["<title>C语言刷题本 · 翁恺 MOOC 习题集</title>", "<title>C语言刷题本 · 本地离线刷题</title>"],
    ["<small>翁恺 MOOC 习题集 · 38 题</small>", "<small>本地离线刷题集 · 38 题</small>"],
    ["C语言刷题本 — 浙江大学翁恺《C语言程序设计》MOOC 配套习题集（38 题，题面为学习改写版）",
     "C语言刷题本 — 本地离线刷题集（38 题，题面为学习改写版）"]
]);

fix(path.join(root, "js", "app.js"), [
    ["题目源自浙江大学翁恺《C语言程序设计》MOOC 配套习题集（经 PTA / PAT 平台），题面为本地学习改写版。",
     "题面为学习改写版，仅用于本地离线练习。"]
]);

fix(path.join(root, "README.md"), [
    ["# C语言刷题本 · 浙江大学翁恺《C语言程序设计》MOOC 习题集（38 题）",
     "# C语言刷题本 · 本地离线刷题集（38 题）"],
    ["题目为翁恺老师 MOOC 配套习题集（PTA / PAT 平台，编号 02-0 ~ 11-1 共 38 题），题面按本地学习需要改写，原题作者信息随题保留。",
     "共 38 题（编号 1 ~ 38），题面为本地学习改写版，原题作者信息随题保留；题号从 1 开始连续编号，便于后续扩展其他题目。"],
    ["翁恺 MOOC 习题集 · 38 题", "本地离线刷题集 · 38 题"],
    ["C语言刷题本 — 浙江大学翁恺《C语言程序设计》MOOC 配套习题集（38 题，题面为学习改写版）",
     "C语言刷题本 — 本地离线刷题集（38 题，题面为学习改写版）"],
    ["题目源自浙江大学翁恺《C语言程序设计》MOOC 配套习题集（经 PTA / PAT 平台），题面为本地学习改写版。",
     "题面为学习改写版，仅用于本地离线练习。"],
    ["题目清单与题面：对照 MOOC 习题集公开资料（PTA/PAT 平台存档、多篇公开题解）整理改写",
     "题目清单与题面：对照公开习题资料（PTA/PAT 平台存档、公开题解）整理改写"]
]);

// 复查
function walk(d, acc) {
    for (const f of fs.readdirSync(d)) {
        const p = path.join(d, f);
        const st = fs.statSync(p);
        if (st.isDirectory()) {
            if (f !== "node_modules" && f !== "C_PAT-master") walk(p, acc);
        } else if (/\.(html|js|css|md)$/.test(f)) {
            const s = fs.readFileSync(p, "utf8");
            if (s.includes("翁恺")) acc.push(path.relative(root, p));
        }
    }
    return acc;
}
const remain = walk(root, []).filter(function (p) { return p.indexOf("tests" + path.sep + "renumber.js") === -1; });
console.log(remain.length ? "仍有残留: " + remain.join(", ") : "全部清理完成 ✓");
