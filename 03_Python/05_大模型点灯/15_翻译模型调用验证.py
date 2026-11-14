# -*- coding: utf-8 -*-
"""验证 doubao-seed-translation-250915 的正确调用方式（Responses API + translation_options）"""
import os
import sys
import json
import urllib.request

try:
    sys.stdout.reconfigure(encoding="utf-8", errors="replace")
except Exception:
    pass

BASE = "https://ark.cn-beijing.volces.com/api/v3"
key = os.getenv("DS_API_KEY")
if not key:
    sys.exit("DS_API_KEY 未设置")


def post(body):
    req = urllib.request.Request(
        BASE + "/responses",
        data=json.dumps(body).encode("utf-8"),
        headers={"Authorization": "Bearer " + key, "Content-Type": "application/json"},
    )
    try:
        with urllib.request.urlopen(req, timeout=120) as r:
            return r.status, r.read().decode("utf-8", "replace")
    except Exception as e:  # noqa: BLE001
        detail = ""
        if hasattr(e, "read"):
            try:
                detail = e.read().decode("utf-8", "replace")
            except Exception:
                pass
        return getattr(e, "code", "ERR"), detail or str(e)


def show(name, body):
    code, text = post(body)
    print("=" * 70)
    print("[{}] HTTP {}".format(name, code))
    print(text[:1200])
    return code == 200


# 方式1：Responses API + input_text + translation_options（官方示例格式）
v1 = {
    "model": "doubao-seed-translation-250915",
    "input": [
        {"role": "user", "content": [
            {"type": "input_text",
             "text": "今天天气不错，我们去公园散步吧。",
             "translation_options": {"source_language": "zh", "target_language": "en"}}
        ]}
    ],
}

# 方式2：同上，不带 source_language（只给必填的 target_language）
v2 = {
    "model": "doubao-seed-translation-250915",
    "input": [
        {"role": "user", "content": [
            {"type": "input_text",
             "text": "Hello, nice to meet you.",
             "translation_options": {"target_language": "zh"}}
        ]}
    ],
}

ok1 = show("方式1 zh->en 带source_language", v1)
ok2 = show("方式2 en->zh 仅target_language", v2)
print("\n结论：方式1 {}；方式2 {}".format("可用" if ok1 else "不可用", "可用" if ok2 else "不可用"))