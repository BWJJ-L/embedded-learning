import sounddevice as sd
import numpy as np

def record_audio(filename="audio_record.pcm",
                 duration=5,
                 rate=16000,
                 channels=1,
                 dtype='int16'):
    """
    录音并保存为 PCM 文件
    :param filename: 保存的文件名
    :param duration: 录音时长（秒）
    :param rate: 采样率
    :param channels: 声道数（1=单声道）
    :param dtype: 数据类型
    :return: 录音数据（numpy数组）
    """
    
    print(f"开始录音{duration}秒...")
    # 录音
    audio = sd.rec(int(duration * rate),  # 录音时长，单位采样点
                samplerate=rate,  # 采样率，单位Hz
                channels=channels, # 单声道，录音一般为单声道
                dtype=dtype,  # 数据类型，16位有符号整数
            )
    # 等待录音完成
    sd.wait()
    print("录音完成")

    # 保存pcm文件格式
    audio.astype(dtype).tofile(filename)
    print(f"音频数据已保存到{filename}")
    
    
def play_audio(filename="audio_record.pcm",
               rate=16000,
               dtype='int16'):
    """
    播放 PCM 文件
    :param filename: 要播放的文件名
    :param rate: 采样率
    :param dtype: 数据类型
    """  
    # 读取pcm文件
    audio = np.fromfile(filename, dtype=dtype)
    print(f"音频数据已读取，共{len(audio)}个采样点")

    # 播放音频
    sd.play(audio, samplerate=rate)
    sd.wait()
    print("音频播放完成")
    

if __name__ == '__main__':
    FILENAME = "record_audio.pcm"
    record_audio(FILENAME, 3) # 音频录入
    play_audio(FILENAME) # 音频播放
    