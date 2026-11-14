import os

import requests

# 凭据从环境变量读取，不硬编码进源码（本仓库是公开仓库）
BAIDU_API_KEY = os.getenv("BAIDU_API_KEY")
if not BAIDU_API_KEY:
    raise ValueError("环境变量 BAIDU_API_KEY 未设置")


def text_to_speech(
    text: str,
    spd: int = 5,          # 语速 0-15，默认 5
    pit: int = 5,          # 音调 0-15，默认 5
    vol: int = 5,          # 音量 0-15，默认 5
    per: int = 4139,       # 发音人，默认 4100（度小宇）
    aue: int = 4,          # 音频格式：3=mp3, 4=pcm-16k, 6=pcm-8k
    lan: str = "zh",       # 语言
    ctp: int = 1,          # 客户端类型，固定 1
    cuid: str = "2V3tKcCBgl9yXr9ft8IfVEDT6640DvKM",
    token: str = "",       # 若用 Authorization 可留空
) -> bytes:
    """
    调用百度语音合成接口，返回音频二进制数据。

    参数：
        text  : 要合成的文本
        spd   : 语速（0–15，默认 5）
        pit   : 音调（0–15，默认 5）
        vol   : 音量（0–15，默认 5）
        per   : 发音人（4100 是“度小宇”等特定音色）
        aue   : 音频格式（3 = mp3，4 = pcm-16k，6 = pcm-8k）
        lan   : 语言（默认 zh）
        ctp   : 客户端类型（固定 1）
        cuid  : 客户端唯一标识
        token : 若使用 Bearer Token 鉴权可留空
    """
    url = "https://tsn.baidu.com/text2audio"

    payload = {
        "tex": text,
        "tok": token,
        "cuid": cuid,
        "ctp": str(ctp),
        "lan": lan,
        "spd": str(spd),
        "pit": str(pit),
        "vol": str(vol),
        "per": str(per),
        "aue": str(aue),
    }

    headers = {
        "Content-Type": "application/x-www-form-urlencoded",
        "Accept": "*/*",
        "cuid": cuid,
        "Authorization": f"Bearer {BAIDU_API_KEY}",
    }

    # requests 会自动对 dict 做 urlencode，并按 utf-8 编码
    response = requests.post(url, headers=headers, data=payload)

    content_type = response.headers.get("Content-Type", "")
    # 出错时百度会返回 json，而不是音频
    if "audio" not in content_type and "json" in content_type:
        raise RuntimeError(f"TTS 接口返回错误：{response.text}")

    # 音频是二进制数据，必须用 content，不能用 text
    return response.content


def main():
    text = "超微哥爱打瓦，叫妈妈"

    audio_bytes = text_to_speech(
        text,
        spd=5,
        pit=5,
        vol=5,
        per=4139,   
        aue=4,      # pcm-16k
    )

    output_file = "record_audio.pcm"
    with open(output_file, "wb") as f:
        f.write(audio_bytes)

    print(f"已保存 PCM 音频：{output_file}（{len(audio_bytes)} 字节）")


if __name__ == "__main__":
    main()