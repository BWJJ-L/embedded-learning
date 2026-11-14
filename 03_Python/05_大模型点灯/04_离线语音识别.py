import sherpa_onnx
import soundfile as sf

# ==================== 1. 加载 SenseVoice 离线识别模型 ====================
model_dir = "models/sherpa-onnx-sense-voice-zh-en-ja-ko-yue-int8-2024-07-17"
recognizer = sherpa_onnx.OfflineRecognizer.from_sense_voice(
    model=f"{model_dir}/model.int8.onnx",   # ONNX模型文件路径（int8量化版，体积更小、推理更快）
    tokens=f"{model_dir}/tokens.txt",       # 词表/子词单元映射文件，模型输出token id→文字的对照表
    use_itn=True,                           # Inverse Text Normalization：将口语化文字转为书面格式
                                            #   例："一百二十三块五" → "123.5元"；关闭则保留原始转写
)

# ==================== 2. 读取裸 PCM 音频文件 ====================
SAMPLE_RATE = 16000  # 采样率(Hz)，必须与录音设备实际采样率一致，否则语速/音调异常
FILENAME = "audio_record.pcm"

with sf.SoundFile(
    FILENAME,             # 裸PCM文件路径（无WAV头，纯音频数据）
    mode="r",             # 只读模式
    format="RAW",         # 指定为原始二进制格式
    samplerate=SAMPLE_RATE,  # 告知soundfile该PCM的采样率（RAW格式无元数据，必须手动指定）
    channels=1,           # 声道数：1=单声道；SenseVoice仅支持单声道输入
    subtype="PCM_16",     # 量化位深：PCM_16=16bit有符号整数；需与录音时编码一致
                          #   常见可选：PCM_16 / PCM_24 / PCM_32 / FLOAT
) as f:
    audio = f.read(dtype="float32")  # 读取并自动转换为float32归一化数组([-1.0, 1.0])
                                    # 模型要求float32输入；若dtype不匹配会导致静音或爆音

# ==================== 3. 创建解码流 → 喂入音频 → 获取识别结果 ====================
stream = recognizer.create_stream()          # 创建独立的识别会话（可复用recognizer多次创建）
stream.accept_waveform(SAMPLE_RATE, audio)   # 传入采样率+音频数据；采样率用于内部重采样校验
recognizer.decode_stream(stream)             # 执行全量离线解码（非流式，一次性处理完整音频）

print("识别结果:", stream.result.text)       # .text 返回最终拼接的纯文本字符串
                                            # 如需带时间戳的分句结果，可用 stream.result.timestamps