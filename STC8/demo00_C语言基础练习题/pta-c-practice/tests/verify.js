"use strict";
/* ============================================================
 * 题库验证脚本（三语言版）：
 *  1. 校验 questions.js / solutions-extra.js 的结构完整性
 *  2. 将每题参考答案实跑比对全部 sample/hidden 用例：
 *     - C / C++   → JSCPP 解释器
 *     - Python    → 本机 CPython（python xxx.py，stdin 送入）
 *  判定前做行末空白/尾部空行归一化，与线上判题一致。
 * 用法：node verify.js
 * ============================================================ */
const fs = require("fs");
const vm = require("vm");
const path = require("path");
const { spawnSync } = require("child_process");
/* 优先加载补丁版引擎（tests/build-engine.js 生成） */
let JSCPP;
try {
  JSCPP = require("./engine-src/commonjs.js");
  console.log("（使用补丁版引擎 tests/engine-src）");
} catch (e) {
  JSCPP = require("JSCPP");
  console.log("（使用 node_modules 原始 JSCPP）");
}

/* ---------- 载入题库 ---------- */
const root = path.join(__dirname, "..");
const sandbox = { window: {}, console };
vm.runInNewContext(fs.readFileSync(path.join(root, "js", "questions.js"), "utf8"), sandbox, { filename: "questions.js" });
vm.runInNewContext(fs.readFileSync(path.join(root, "js", "solutions-extra.js"), "utf8"), sandbox, { filename: "solutions-extra.js" });
const BANK = sandbox.window.QUESTION_BANK;
const EXTRA = sandbox.window.REFERENCE_EXTRA || {};

/* ---------- 归一化（与线上判题同一规则） ---------- */
function normalize(s) {
  return String(s).replace(/\r/g, "").split("\n").map(l => l.replace(/[ \t]+$/, "")).join("\n").replace(/\n+$/, "");
}

/* ---------- 运行器 ---------- */
let tmpSeq = 0;
function runC(code, input) {
  let output = "";
  const t0 = Date.now();
  try {
    JSCPP.run(code, input, { stdio: { write: s => { output += s; } } });
    return { ok: true, output, ms: Date.now() - t0 };
  } catch (e) {
    return { ok: false, output, error: (e && e.message) || String(e), ms: Date.now() - t0 };
  }
}
function runPython(code, input) {
  const tmp = path.join(__dirname, `__ref_${++tmpSeq}.py`);
  fs.writeFileSync(tmp, code, "utf8");
  const t0 = Date.now();
  try {
    const r = spawnSync("python", [tmp], { input: String(input), encoding: "utf8", timeout: 15000 });
    const ok = r.status === 0;
    return { ok, output: r.stdout || "", error: ok ? "" : (r.stderr || "exit " + r.status).split("\n").filter(l => l.trim()).pop(), ms: Date.now() - t0 };
  } finally {
    try { fs.unlinkSync(tmp); } catch (e) { /* 忽略 */ }
  }
}
function runLang(lang, code, input) {
  return lang === "python" ? runPython(code, input) : runC(code, input);
}

/* ---------- 结构校验 ---------- */
const structErrors = [];
const ids = new Set();
for (const q of BANK) {
  const label = q.code || ("id=" + q.id);
  for (const f of ["id", "code", "title", "difficulty", "tags", "description", "examples",
    "constraints", "hints", "solution", "referenceSolution", "sampleCases", "hiddenCases"]) {
    if (q[f] === undefined) structErrors.push(`${label}: 缺少字段 ${f}`);
  }
  if (ids.has(q.id)) structErrors.push(`${label}: id 重复`);
  ids.add(q.id);
  if (!["easy", "medium", "hard"].includes(q.difficulty)) structErrors.push(`${label}: 难度非法`);
  if (!q.referenceSolution.includes("int main")) structErrors.push(`${label}: C 参考答案不是完整程序`);
  for (const lang of ["c", "cpp", "python"]) {
    if (!q.langTemplates || !q.langTemplates[lang]) structErrors.push(`${label}: 缺少 ${lang} 模板`);
  }
  const e = EXTRA[q.code];
  if (!e) structErrors.push(`${label}: REFERENCE_EXTRA 缺项`);
  else {
    if (!e.cpp || !e.cpp.includes("main")) structErrors.push(`${label}: C++ 参考答案缺失`);
    if (!e.python || !e.python.trim()) structErrors.push(`${label}: Python 参考答案缺失`);
  }
  if (q.examples && q.examples[0] && q.sampleCases && q.sampleCases[0]) {
    const a = q.examples[0], b = q.sampleCases[0];
    if (normalize(a.input) !== normalize(b.input) || normalize(a.output) !== normalize(b.expected)) {
      structErrors.push(`${label}: examples[0] 与 sampleCases[0] 不一致`);
    }
  }
}
if (BANK.length !== 38) structErrors.push(`题库数量为 ${BANK.length}，应为 38`);

/* ---------- 三语言实跑验证 ---------- */
const LANGS = [
  ["C", q => q.referenceSolution, "c"],
  ["C++", q => (EXTRA[q.code] || {}).cpp, "cpp"],
  ["Python", q => (EXTRA[q.code] || {}).python, "python"]
];

const stat = {};
const failures = [];
let total = 0, pass = 0;
const maxMs = { val: 0, where: "" };

for (const q of BANK) {
  const cases = [
    ...q.sampleCases.map(c => ({ ...c, kind: "sample" })),
    ...q.hiddenCases.map(c => ({ ...c, kind: "hidden" }))
  ];
  for (const [name, getSrc, langKey] of LANGS) {
    stat[name] = stat[name] || { total: 0, pass: 0 };
    const src = getSrc(q);
    for (const c of cases) {
      total++;
      stat[name].total++;
      const r = runLang(langKey, src, c.input);
      if (r.ms > maxMs.val) maxMs.val = r.ms, maxMs.where = `${q.code} ${name} (${c.kind})`;
      const ok = r.ok && normalize(r.output) === normalize(c.expected);
      if (ok) { pass++; stat[name].pass++; continue; }
      failures.push({ problem: `${q.code} ${q.title}`, lang: name, kind: c.kind, input: c.input, expected: c.expected, actual: r.output, error: r.error || "", ms: r.ms });
    }
  }
}

/* ---------- 报告 ---------- */
console.log("=== 结构校验 ===");
console.log(structErrors.length ? structErrors.map(e => "  " + e).join("\n") : "全部通过");
console.log("\n=== 三语言参考答案实跑验证 ===");
console.log(`用例总数: ${total}，通过: ${pass}，失败: ${total - pass}`);
for (const name of Object.keys(stat)) {
  console.log(`  ${name}: ${stat[name].pass}/${stat[name].total}`);
}
console.log(`单次执行最耗时: ${maxMs.val}ms @ ${maxMs.where}`);

if (structErrors.length || failures.length) {
  console.log("\n=== 失败明细 ===");
  for (const f of failures) {
    console.log(`\n[${f.problem}] ${f.lang} ${f.kind} (${f.ms}ms)`);
    if (f.error) console.log("  错误: " + f.error);
    console.log("  输入: " + JSON.stringify(f.input));
    console.log("  期望: " + JSON.stringify(f.expected));
    console.log("  实际: " + JSON.stringify(f.actual));
  }
  process.exitCode = 1;
} else {
  console.log("\n三种语言全部用例验证通过 ✓");
}
