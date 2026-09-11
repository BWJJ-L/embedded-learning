"use strict";
/* 一次性改造脚本：
 * 1. 题号 code/slug 从 "02-0" 系列改为顺序编号 "1".."38"（questions.js + solutions-extra.js）
 * 2. 删除题目数据与注释中的“翁恺”字样
 * 运行：node tests/renumber.js
 */
const fs = require("fs");
const path = require("path");
const root = path.join(__dirname, "..");

const oldCodes = [
    "02-0", "02-1", "02-2", "02-3", "02-4",
    "03-0", "03-1", "03-2", "03-3", "03-4",
    "04-0", "04-1", "04-2", "04-3", "04-4",
    "05-0", "05-1", "05-2", "05-3",
    "06-0", "06-1", "06-2", "06-3",
    "07-0", "07-1", "07-2", "07-3",
    "08-0", "08-1", "08-2", "08-3",
    "10-0", "10-1", "10-2", "10-3", "10-4",
    "11-0", "11-1"
];
const map = {};
oldCodes.forEach((c, i) => { map[c] = String(i + 1); });

let q = fs.readFileSync(path.join(root, "js", "questions.js"), "utf8");
let nCode = 0, nSlug = 0;
oldCodes.forEach((c) => {
    q = q.split("code: '" + c + "'").join("code: '" + map[c] + "'");
    q = q.split("slug: '" + c + "'").join("slug: '" + map[c] + "'");
});
// 计数验证
nCode = (q.match(/code: '/g) || []).length;
nSlug = (q.match(/slug: '/g) || []).length;
// 删除翁恺相关表述
q = q.split("浙江大学翁恺《C语言程序设计》MOOC 配套习题集（38 题）").join("本地代码刷题集（38 题）");
q = q.split("题面依据原题改写，作者信息随题保留；判题采用\"标准输入 → 标准输出\"模式。").join("题面依据原题改写；判题采用\"标准输入 → 标准输出\"模式。");
fs.writeFileSync(path.join(root, "js", "questions.js"), q, "utf8");
console.log("questions.js: code/slug 共", nCode, "/", nSlug, "处，已重编号");

let s = fs.readFileSync(path.join(root, "js", "solutions-extra.js"), "utf8");
oldCodes.forEach((c) => {
    s = s.split("'" + c + "': {").join("'" + map[c] + "': {");
});
s = s.split("/* ============================================================\n * C++ 与 Python 参考答案").join("/* ============================================================\n * C++ 与 Python 参考答案");
fs.writeFileSync(path.join(root, "js", "solutions-extra.js"), s, "utf8");
console.log("solutions-extra.js: 键已重编号");

// 自检：载入并核对
const vm = require("vm");
const sb = { window: {}, console };
vm.runInNewContext(fs.readFileSync(path.join(root, "js", "questions.js"), "utf8"), sb);
vm.runInNewContext(fs.readFileSync(path.join(root, "js", "solutions-extra.js"), "utf8"), sb);
const bank = sb.window.QUESTION_BANK;
const extra = sb.window.REFERENCE_EXTRA;
let bad = [];
bank.forEach(function (x) {
    if (x.code !== String(x.id)) bad.push("id=" + x.id + " code=" + x.code);
    if (!extra[x.code]) bad.push(x.code + " 缺 REFERENCE_EXTRA");
});
const ka = JSON.stringify(bank.map(function (x) { return x.code; }));
if (ka.indexOf("翁恺") !== -1 || fs.readFileSync(path.join(root, "js", "questions.js"), "utf8").indexOf("翁恺") !== -1) bad.push("questions.js 仍有 翁恺");
console.log(bad.length ? "异常: " + bad.join("; ") : "自检通过：38 题编号 = 1..38，REFERENCE_EXTRA 全部对上，无翁恺字样");
