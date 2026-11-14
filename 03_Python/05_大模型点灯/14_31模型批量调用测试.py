# -*- coding: utf-8 -*-
"""
火山方舟 31 个模型批量连通性测试（一次性跑完，输出 JSON 结果）

测试策略（对每个模型依次执行）：
  阶段 A  最朴素调用：只有 model + messages                → 成功 = "一次性调用成功"
  阶段 B  对 A 失败的模型依次尝试 3 种改法：
          B1 关闭深度思考 thinking={"type":"disabled"}
          B2 加大 max_tokens=512
          B3 改用 Responses API（/v3/responses）
          → 任一成功 = "需要额外代码更改才能调用"（记录具体改法）
  阶段 C  对仍失败的模型，尝试候选备用 Model ID（防止 ID 映射错误）
  全部失败 → "不能调用"（记录原始报错）

用法：D:\\Develop\\Python\\python39\\python.exe 14_31模型批量调用测试.py
依赖：环境变量 DS_API_KEY、openai SDK
"""
import os
import sys
import json
import time
import urllib.request

try:
    sys.stdout.reconfigure(encoding="utf-8", errors="replace")
except Exception:
    pass

from openai import OpenAI

BASE = "https://ark.cn-beijing.volces.com/api/v3"
RESULT_FILE = os.path.join(os.path.dirname(os.path.abspath(__file__)), "ark_31模型测试结果.json")

# (控制台展示名, 首选 Model ID)
MODELS = [
    ("Doubao-Seed-2.1-lite",        "doubao-seed-2-1-lite-260915"),
    ("DeepSeek-V4.1-Flash",         "deepseek-v4-1-flash-260910"),
    ("GLM-5.3-Flash",               "glm-5-3-flash-260828"),
    ("DeepSeek-V4-Pro正式版",        "deepseek-v4-pro-ga-260813"),
    ("DeepSeek-V4-Flash正式版",      "deepseek-v4-flash-ga-260731"),
    ("GLM-5.2",                     "glm-5-2-260617"),
    ("Doubao-Seed-Evolving",        "doubao-seed-evolving"),
    ("Doubao-Seed-2.1-turbo",       "doubao-seed-2-1-turbo-260628"),
    ("Doubao-Seed-2.0-mini",        "doubao-seed-2-0-mini-260428"),
    ("Doubao-Seed-2.0-lite",        "doubao-seed-2-0-lite-260428"),
    ("Doubao-Seed-2.1-pro",         "doubao-seed-2-1-pro-260915"),
    ("DeepSeek-V4-Flash",           "deepseek-v4-flash-260425"),
    ("DeepSeek-V4-pro",             "deepseek-v4-pro-260425"),
    ("Doubao-Seed-2.0-Code",        "doubao-seed-2-0-code-preview-260215"),
    ("Doubao-Seed-1.8",             "doubao-seed-1-8-251228"),
    ("Doubao-Seed-2.0-pro",         "doubao-seed-2-0-pro-260215"),
    ("Doubao-Seed-Character",       "doubao-seed-character-260628"),
    ("GLM-4.7",                     "glm-4-7-251222"),
    ("DeepSeek-V3.2",               "deepseek-v3-2-251201"),
    ("Doubao-Seed-Code",            "doubao-seed-code-preview-251028"),
    ("Doubao-Seed-Translation",     "doubao-seed-translation-250915"),
    ("Doubao-Seed-1.6-vision",      "doubao-seed-1-6-vision-250815"),
    ("Doubao-Seed-1.6-flash",       "doubao-seed-1-6-flash-250828"),
    ("Doubao-Seed-1.6",             "doubao-seed-1-6-251015"),
    ("Doubao-1.5-vision-lite",      "doubao-1-5-vision-lite-250315"),
    ("Doubao-1.5-vision-pro-32k",   "doubao-1-5-vision-pro-32k-250115"),
    ("Doubao-1.5-lite-32k",         "doubao-1-5-lite-32k-250115"),
    ("Doubao-1.5-pro-32k",          "doubao-1-5-pro-32k-250115"),
    ("Doubao-pro-32k",              "doubao-pro-32k-241215"),
    ("Doubao-lite-4k",              "doubao-lite-4k-240828"),
    ("Doubao-lite-32k",             "doubao-lite-32k-240828"),
]

# 首选 ID 失败时，尝试的候选 ID（只在报错疑似"模型不存在/找不到"时使用）
ALT_IDS = {
    "doubao-lite-4k-240828":           ["doubao-lite-4k", "doubao-lite-4k-character-240828"],
    "doubao-seed-character-260628":    ["doubao-seed-character-251128"],
    "doubao-seed-1-8-251228":          ["doubao-seed-1-8-251215"],
    "doubao-seed-2-0-code-preview-260215": ["doubao-seed-2-0-code-preview-260428"],
}

api_key = os.getenv("DS_API_KEY")
if not api_key:
    sys.exit("环境变量 DS_API_KEY 未设置")

client = OpenAI(api_key=api_key, base_url=BASE, timeout=120, max_retries=0)
PROMPT = [{"role": "user", "content": "你好"}]


def call_plain(model, **extra):
    """最朴素的 chat.completions 调用"""
    t0 = time.time()
    try:
        r = client.chat.completions.create(model=model, messages=PROMPT, **extra)
        txt = (r.choices[0].message.content or "").strip() if r.choices else ""
        return {"ok": True, "sec": round(time.time() - t0, 1), "reply": txt[:50]}
    except Exception as e:  # noqa: BLE001
        return {"ok": False, "sec": round(time.time() - t0, 1),
                "error": "{}: {}".format(type(e).__name__, str(e)[:400])}


def call_responses_api(model):
    """改用 Responses API（原生 HTTP，避免 SDK 版本差异）"""
    t0 = time.time()
    try:
        req = urllib.request.Request(
            BASE + "/responses",
            data=json.dumps({"model": model, "input": "你好"}).encode("utf-8"),
            headers={"Authorization": "Bearer " + api_key, "Content-Type": "application/json"},
        )
        with urllib.request.urlopen(req, timeout=120) as resp:
            body = resp.read().decode("utf-8", "replace")
        return {"ok": True, "sec": round(time.time() - t0, 1), "reply": body[:80]}
    except Exception as e:  # noqa: BLE001
        detail = ""
        if hasattr(e, "read"):
            try:
                detail = e.read().decode("utf-8", "replace")[:400]
            except Exception:
                pass
        return {"ok": False, "sec": round(time.time() - t0, 1),
                "error": "{}: {}".format(type(e).__name__, detail or str(e)[:400])}


def looks_like_id_error(err):
    """报错是否疑似"模型 ID 不存在"（决定要不要试候选 ID）"""
    if not err:
        return False
    keys = ["not found", "notfound", "does not exist", "invalidendpoint", "不支持该模型", "模型不存在"]
    low = err.lower()
    return any(k in low for k in keys)


def main():
    results = []
    for idx, (console_name, model_id) in enumerate(MODELS, 1):
        rec = {"no": idx, "console": console_name, "model_id": model_id,
               "stageA": None, "stageB": {}, "alts": {}, "final": "", "note": ""}
        print("[{:>2}/{}] {}  ->  {}".format(idx, len(MODELS), console_name, model_id), flush=True)

        # ---------- 阶段 A：最朴素调用 ----------
        a = call_plain(model_id)
        rec["stageA"] = a
        if a["ok"]:
            rec["final"] = "ok_first"
            rec["note"] = "最朴素写法一次成功；回复：{}".format(a["reply"])
            print("    A 成功（{}s）：{}".format(a["sec"], a["reply"]), flush=True)
            results.append(rec)
            continue
        print("    A 失败（{}s）：{}".format(a["sec"], a["error"]), flush=True)

        # ---------- 阶段 B：3 种改法 ----------
        b1 = call_plain(model_id, extra_body={"thinking": {"type": "disabled"}})
        rec["stageB"]["B1_关闭thinking"] = b1
        print("    B1 关闭thinking：{}".format("成功" if b1["ok"] else "失败 " + b1["error"]), flush=True)
        if b1["ok"]:
            rec["final"] = "ok_changed"
            rec["note"] = '需要加 extra_body={"thinking":{"type":"disabled"}}；回复：{}'.format(b1["reply"])
            results.append(rec)
            continue

        b2 = call_plain(model_id, max_tokens=512)
        rec["stageB"]["B2_max_tokens512"] = b2
        print("    B2 max_tokens=512：{}".format("成功" if b2["ok"] else "失败 " + b2["error"]), flush=True)
        if b2["ok"]:
            rec["final"] = "ok_changed"
            rec["note"] = "需要显式指定 max_tokens；回复：{}".format(b2["reply"])
            results.append(rec)
            continue

        b3 = call_responses_api(model_id)
        rec["stageB"]["B3_ResponsesAPI"] = b3
        print("    B3 Responses API：{}".format("成功" if b3["ok"] else "失败 " + b3["error"]), flush=True)
        if b3["ok"]:
            rec["final"] = "ok_changed"
            rec["note"] = "需要用 Responses API（/v3/responses）调用"
            results.append(rec)
            continue

        # ---------- 阶段 C：候选备用 ID ----------
        if looks_like_id_error(a["error"]):
            for alt in ALT_IDS.get(model_id, []):
                alt_a = call_plain(alt)
                rec["alts"][alt] = alt_a
                print("    C 候选ID {}：{}".format(alt, "成功" if alt_a["ok"] else "失败 " + alt_a["error"]), flush=True)
                if alt_a["ok"]:
                    rec["final"] = "ok_changed"
                    rec["note"] = "首选 ID 无效，改用 {} 可调用".format(alt)
                    break

        if not rec["final"]:
            rec["final"] = "dead"
            rec["note"] = a["error"]
        results.append(rec)

    # ---------- 汇总输出 ----------
    summary = {"ok_first": [], "ok_changed": [], "dead": []}
    for rec in results:
        summary[rec["final"]].append(rec["console"])

    print("\n" + "=" * 60)
    print("一次性调用成功：{} 个".format(len(summary["ok_first"])))
    for n in summary["ok_first"]:
        print("  [OK] " + n)
    print("需额外代码更改才能调用：{} 个".format(len(summary["ok_changed"])))
    for n in summary["ok_changed"]:
        print("  [CHG] " + n)
    print("不能调用：{} 个".format(len(summary["dead"])))
    for n in summary["dead"]:
        print("  [X] " + n)
    print("=" * 60)

    with open(RESULT_FILE, "w", encoding="utf-8") as f:
        json.dump({"tested_at": time.strftime("%Y-%m-%d %H:%M:%S"),
                   "base_url": BASE, "results": results, "summary": summary},
                  f, ensure_ascii=False, indent=2)
    print("结果已写入：" + RESULT_FILE)


if __name__ == "__main__":
    main()