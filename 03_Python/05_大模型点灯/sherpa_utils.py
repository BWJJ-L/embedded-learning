"""
sherpa_utils.py
基于 sherpa-onnx 的语音识别 / 语音合成工具模块。

对外仅暴露两个函数：
    - speech_to_text(...)  离线语音识别（录音或传音频数组）
    - text_to_speech(...)  离线语音合成（可播放、可保存 wav）
"""

import os
import numpy as np
import sherpa_onnx
import sounddevice as sd

# ---------------- 默认配置 ----------------
ASR_MODEL_DIR = "models/sherpa-onnx-sense-voice-zh-en-ja-ko-yue-int8-2024-07-17"
TTS_MODEL_DIR = "models/vits-melo-tts-zh_en"

SAMPLING_RATE = 16000       # 识别采样率
DEFAULT_DURATION = 5.0      # 默认录音时长（秒）

# ---------------- 模型缓存（避免重复加载） ----------------
_asr_recognizer = None
_tts_engine = None


def _get_recognizer(model_dir=ASR_MODEL_DIR, use_itn=True):
    """懒加载 SenseVoice 识别器。"""
    global _asr_recognizer
    if _asr_recognizer is None:
        _asr_recognizer = sherpa_onnx.OfflineRecognizer.from_sense_voice(
            model=os.path.join(model_dir, "model.int8.onnx"),
            tokens=os.path.join(model_dir, "tokens.txt"),
            use_itn=use_itn,
        )
    return _asr_recognizer


def _get_tts(model_dir=TTS_MODEL_DIR, num_threads=2, provider="cpu"):
    """懒加载 VITS TTS 引擎。"""
    global _tts_engine
    if _tts_engine is None:
        rule_fsts = ",".join([
            os.path.join(model_dir, "phone.fst"),
            os.path.join(model_dir, "date.fst"),
            os.path.join(model_dir, "number.fst"),
            os.path.join(model_dir, "new_heteronym.fst"),
        ])
        config = sherpa_onnx.OfflineTtsConfig(
            model=sherpa_onnx.OfflineTtsModelConfig(
                vits=sherpa_onnx.OfflineTtsVitsModelConfig(
                    model=os.path.join(model_dir, "model.onnx"),
                    lexicon=os.path.join(model_dir, "lexicon.txt"),
                    tokens=os.path.join(model_dir, "tokens.txt"),
                    dict_dir=os.path.join(model_dir, "dict"),
                ),
                provider=provider,
                num_threads=num_threads,
            ),
            rule_fsts=rule_fsts,
        )
        _tts_engine = sherpa_onnx.OfflineTts(config)
    return _tts_engine


# =========================================================
# 对外函数 1：语音识别
# =========================================================
def speech_to_text(
    audio=None,
    duration=DEFAULT_DURATION,
    sampling_rate=SAMPLING_RATE,
    model_dir=ASR_MODEL_DIR,
    use_itn=True,
    verbose=True,
):
    """
    离线语音识别。

    参数:
        audio:         已录好的音频数据（int16 的 numpy 数组）。
                       为 None 时，自动从麦克风录制 duration 秒。
        duration:      录音时长（秒），仅在 audio 为 None 时生效。
        sampling_rate: 采样率，默认 16000。
        model_dir:     模型目录。
        use_itn:       是否使用 ITN（逆文本正则化，输出更书面化）。
        verbose:       是否打印过程信息。

    返回:
        识别到的文本字符串。
    """
    # 1) 未传入音频则现场录音
    if audio is None:
        if verbose:
            print(f"开始录音 {duration} 秒...")
        audio = sd.rec(
            int(duration * sampling_rate),
            samplerate=sampling_rate,
            channels=1,
            dtype="int16",
        )
        sd.wait()
        if verbose:
            print("录音完成")

    # 2) 解码
    recognizer = _get_recognizer(model_dir=model_dir, use_itn=use_itn)
    stream = recognizer.create_stream()
    stream.accept_waveform(sampling_rate, audio)
    recognizer.decode_stream(stream)

    text = stream.result.text
    if verbose:
        print(f"识别结果：{text}")
    return text


# =========================================================
# 对外函数 2：语音合成
# =========================================================
def text_to_speech(
    text,
    model_dir=TTS_MODEL_DIR,
    sid=0,
    speed=1.0,
    play=True,
    save_path=None,
    provider="cpu",
    num_threads=2,
    verbose=True,
):
    """
    离线语音合成。

    参数:
        text:        待合成的文本（支持中英文混合）。
        model_dir:   模型目录。
        sid:         说话人 id（0 通常为默认说话人）。
        speed:       语速，1.0 为正常。
        play:        是否用 sounddevice 立即播放。
        save_path:   若不为 None，将合成的 wav 保存到该路径。
        provider:    推理后端，"cpu" 或 "cuda"。
        num_threads: 线程数。
        verbose:     是否打印过程信息。

    返回:
        (samples, sample_rate)  合成的音频数据（float32 numpy 数组）与采样率。
    """
    tts = _get_tts(model_dir=model_dir, num_threads=num_threads, provider=provider)

    if verbose:
        print(f"正在合成: {text}")
    audio = tts.generate(text, sid=sid, speed=speed)

    samples = np.array(audio.samples, dtype=np.float32)
    sample_rate = audio.sample_rate

    if len(samples) == 0:
        raise RuntimeError("音频合成失败，未生成任何样本。")

    if verbose:
        print(f"合成完成，采样率: {sample_rate}, "
              f"时长: {len(samples) / sample_rate:.2f} 秒")

    # 保存
    if save_path:
        sherpa_onnx.write_wav(save_path, samples, sample_rate)
        if verbose:
            print(f"已保存到: {save_path}")

    # 播放
    if play:
        if verbose:
            print("正在播放...")
        sd.play(samples, sample_rate)
        sd.wait()
        if verbose:
            print("播放结束。")

    return samples, sample_rate


# =========================================================
# 简单演示
# =========================================================
if __name__ == "__main__":
    # 1. 录音 5 秒并识别
    text = speech_to_text(duration=5)
    print("你说的是：", text)

    # 2. 把识别结果反过来合成播放
    if text.strip():
        text_to_speech(text, speed=1.0, play=True)