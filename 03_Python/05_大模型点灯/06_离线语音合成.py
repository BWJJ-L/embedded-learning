import os
import numpy as np
import sherpa_onnx
import sounddevice as sd

def replace_digits(text):
    """将阿拉伯数字0-9替换为中文数字"""
    mapping = str.maketrans('0123456789', '零一二三四五六七八九')
    return text.translate(mapping)

def main():
    model_dir = "models/vits-melo-tts-zh_en"
    
    if not os.path.exists(model_dir):
        print(f"错误: 找不到模型目录 {model_dir}，请检查路径。")
        return

    model_path = os.path.join(model_dir, "model.onnx")
    
    tts_config = sherpa_onnx.OfflineTtsConfig(
        model=sherpa_onnx.OfflineTtsModelConfig(
            vits=sherpa_onnx.OfflineTtsVitsModelConfig(
                model=model_path,
                lexicon=os.path.join(model_dir, "lexicon.txt"),
                tokens=os.path.join(model_dir, "tokens.txt"),
                dict_dir=os.path.join(model_dir, "dict"),
            ),
            provider="cpu",
            num_threads=2,
        ),
    )

    print("正在加载模型...")
    tts = sherpa_onnx.OfflineTts(tts_config)
    print("模型加载完成！")

    # 原始文本
    raw_text = "你好，欢迎使用 Sherpa ONNX。今天的日期是2023年10月1日，电话是12345678。"
    
    # 替换数字
    text = replace_digits(raw_text)
    print(f"处理后文本: {text}")

    print(f"正在合成...")
    audio = tts.generate(text, sid=0, speed=1.0)

    if len(audio.samples) == 0:
        print("错误: 音频合成失败，未生成任何样本。")
        return

    samples = np.array(audio.samples, dtype=np.float32)
    sample_rate = audio.sample_rate

    print(f"合成完成，采样率: {sample_rate}, 时长: {len(samples)/sample_rate:.2f}秒")
    print("正在播放...")
    sd.play(samples, sample_rate)
    sd.wait()
    print("播放结束。")

if __name__ == "__main__":
    main()