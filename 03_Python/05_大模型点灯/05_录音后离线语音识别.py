import sherpa_onnx
import sounddevice as sd

# ==================== 1. 加载 SenseVoice 离线识别模型 ====================
model_dir = "models/sherpa-onnx-sense-voice-zh-en-ja-ko-yue-int8-2024-07-17"
recognizer = sherpa_onnx.OfflineRecognizer.from_sense_voice(
    model=f"{model_dir}/model.int8.onnx",   # int8量化ONNX模型路径，体积小、CPU推理快
    tokens=f"{model_dir}/tokens.txt",       # 词表文件：模型输出token id → 文字的映射表
    use_itn=True,                           # 逆文本正则化：将口语转为书面格式
                                            #   例："一百二十三块五" → "123.5元"
)

SAMPLE_RATE = 16000  # 采样率(Hz)，SenseVoice标准输入为16kHz单声道

# ==================== 2. 麦克风录音 ====================
duration = 5  # 录音时长（秒）
print(f"请开始说话（{duration} 秒）...")

audio = sd.rec(
    int(duration * SAMPLE_RATE),  # 总采样点数 = 时长 × 采样率
    samplerate=SAMPLE_RATE,       # 设备采样率，需与模型要求一致
    channels=1,                   # 单声道；SenseVoice不支持多声道输入
    dtype="int16",                # 16bit有符号整数，sounddevice默认采集格式
)
sd.wait()                         # 阻塞等待录音完成，避免提前进入识别流程

audio = audio.flatten()           # 将多维数组展平为一维数组
print("录音结束，正在识别...")

if audio.size == 0:               # 防御性检查：录音设备异常时避免后续崩溃
    print("没录到声音")
    exit()

# ==================== 3. 离线识别 ====================
stream = recognizer.create_stream()          # 创建独立的识别会话（可复用recognizer多次创建）
stream.accept_waveform(SAMPLE_RATE, audio)   # 传入采样率+音频数据；采样率用于内部重采样校验
recognizer.decode_stream(stream)             # 执行全量离线解码（非流式，一次性处理完整音频）

print("识别结果:", stream.result.text)       # .text 返回最终拼接的纯文本字符串
                                            # 如需带时间戳的分句结果，可用 stream.result.timestamps