
import sounddevice as sd
import numpy as np

RATE = 16000
FILENAME = "record_audio.pcm"

data = np.fromfile(FILENAME, dtype = 'int16')

print("开始播放...")
sd.play(data, samplerate = RATE)
sd.wait()
print("播放结束!!!")

