# 离线语音模型获取说明

`models/` 目录不入版本库（总体积约 727 MB，其中单个 `.onnx` 最大 228 MiB，超过
GitHub 100 MB 单文件硬限制）。克隆本仓库后，按下面命令把模型放回
`03_Python/05_大模型点灯/models/` 即可，代码里的路径是相对路径，不需要改。

## 需要的两个模型

| 模型 | 用途 | 解压后体积 | 代码里的引用位置 |
|---|---|---|---|
| `sherpa-onnx-sense-voice-zh-en-ja-ko-yue-int8-2024-07-17` | 离线语音识别（ASR） | 约 228 MiB | `04_离线语音识别.py`、`05_录音后离线语音识别.py`、`16_.py`、`sherpa_utils.py` |
| `vits-melo-tts-zh_en` | 离线语音合成（TTS） | 约 163 MiB | `06_离线语音合成.py`、`sherpa_utils.py` |

## 下载

在 `03_Python/05_大模型点灯/` 目录下执行（Linux / macOS / Git Bash）：

```bash
mkdir -p models
cd models

# 语音识别模型（SenseVoice，中英日韩粤）
wget https://github.com/k2-fsa/sherpa-onnx/releases/download/asr-models/sherpa-onnx-sense-voice-zh-en-ja-ko-yue-int8-2024-07-17.tar.bz2
tar xvf sherpa-onnx-sense-voice-zh-en-ja-ko-yue-int8-2024-07-17.tar.bz2
rm sherpa-onnx-sense-voice-zh-en-ja-ko-yue-int8-2024-07-17.tar.bz2

# 语音合成模型（MeloTTS 中英）
wget https://github.com/k2-fsa/sherpa-onnx/releases/download/tts-models/vits-melo-tts-zh_en.tar.bz2
tar xvf vits-melo-tts-zh_en.tar.bz2
rm vits-melo-tts-zh_en.tar.bz2
```

Windows PowerShell 下没有 `wget`，用 `curl.exe -LO <URL>` 下载，再用 7-Zip 解压
`.tar.bz2`（资源管理器不支持这种双重压缩格式）。

下载慢的话可用 Hugging Face 镜像，模型同名：
<https://huggingface.co/csukuangfj/sherpa-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17>

## 校验

解压后确认这两个文件存在且大小对得上，缺任何一个代码都会在初始化时报错：

```
models/sherpa-onnx-sense-voice-zh-en-ja-ko-yue-int8-2024-07-17/model.int8.onnx   约 228 MiB
models/vits-melo-tts-zh_en/model.onnx                                            约 163 MiB
```

## 依赖

```bash
pip install sherpa-onnx sounddevice numpy
```

## 关于密钥

`16_.py` 里用到的 OneNET 与 DeepSeek 凭据一律从环境变量读取，源码里没有明文：

```bash
export ONENET_ACCESS_KEY=<设备密钥>
export DS_API_KEY=<API Key>
```

未设置时脚本会直接抛出 `ValueError` 并说明缺哪个变量。OneNET 的 `product_id`
与 `device_name` 有默认值，换设备时用 `ONENET_PRODUCT_ID` / `ONENET_DEVICE_NAME`
覆盖即可。
