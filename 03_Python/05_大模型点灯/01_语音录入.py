
import sounddevice as sd


RATE = 16000    # 采样率
DURATION = 5    # 录音时长
FILENAME = "audio_record.pcm"

print(f"开始录音{DURATION}秒....")
# 一句话录音：直接录 DURATION 秒，返回一个 numpy 数组
data = sd.rec(int(DURATION * RATE), 
              samplerate = RATE,
              channels = 1,
              dtype = 'int16')

sd.wait()
print("录音结束！！！")

# 写入 PCM 文件（int16 数组直接转成原始字节）
data.astype('int16').tofile(FILENAME)
print(f"音频已保存为{FILENAME}")