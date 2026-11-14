# ============================================================
#  智能家居控制面板（最终完整版 · TTS 连续播报修复）
#  GUI + DeepSeek + OneNET + 串口 + TTS队列 + SenseVoice 离线语音交互
#  依赖：pip install openai pyttsx3 paho-mqtt pyserial sounddevice numpy sherpa-onnx
# ============================================================
import json
import time
import base64
import hmac
import hashlib
import threading
import re
import queue
import os
import sys
from urllib.parse import quote

import numpy as np
import sounddevice as sd

import serial
import paho.mqtt.client as mqtt
from openai import OpenAI
import pyttsx3

import tkinter as tk
from tkinter import ttk, scrolledtext


# ==================== 1. 配置 ====================
SERIAL_PORT = "COM6"
SERIAL_BAUD = 115200
SERIAL_TIMEOUT = 0.05

# 凭据一律从环境变量读取，不硬编码进源码（本仓库是公开仓库）
# OneNET：产品 ID / 设备名 / 设备密钥
product_id = os.getenv("ONENET_PRODUCT_ID", "80HI4oNM8e")
device_name = os.getenv("ONENET_DEVICE_NAME", "advance1")
access_key = os.getenv("ONENET_ACCESS_KEY")
if not access_key:
    raise ValueError("环境变量 ONENET_ACCESS_KEY 未设置")

et = int(time.time()) + 10 * 365 * 24 * 3600
hostname = "mqtts.heclouds.com"
port = 1883

pub_topic = f"$sys/{product_id}/{device_name}/thing/property/post"
sub_topic = f"$sys/{product_id}/{device_name}/thing/property/set"
set_reply_topic = f"$sys/{product_id}/{device_name}/thing/property/set_reply"

DEEPSEEK_KEY = os.getenv("DS_API_KEY")
if not DEEPSEEK_KEY:
    raise ValueError("环境变量 DS_API_KEY 未设置")

# 告警阈值
TEMP_HIGH = 30.0
TEMP_LOW = 10.0
HUM_HIGH = 80
HUM_LOW = 20
ALARM_INTERVAL = 300

# ==================== ASR 配置 ====================
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
MODEL_DIR = os.path.join(
    SCRIPT_DIR,
    "./models/sherpa-onnx-sense-voice-zh-en-ja-ko-yue-int8-2024-07-17"
)

ASR_RATE = 16000
ASR_CHANNELS = 1
ASR_DTYPE = "int16"
MAX_RECORD_SECONDS = 30
SILENCE_DURATION = 2.5
SILENCE_THRESHOLD = 500
BLOCK_SIZE = 1024


def generate_token(product_id, access_key, et, method="md5"):
    version = "2018-10-31"
    res = f"products/{product_id}"
    key = base64.b64decode(access_key)
    string_for_sign = f"{et}\n{method}\n{res}\n{version}".encode("utf-8")
    sign = base64.b64encode(
        hmac.new(key, string_for_sign, getattr(hashlib, method)).digest()
    ).decode()
    sign = quote(sign, safe="")
    res = quote(res, safe="")
    return f"version={version}&res={res}&et={et}&method={method}&sign={sign}"


token = generate_token(product_id, access_key, et)


# ==================== 2. 全局状态 ====================
state = {
    "Temperature": None,
    "Humidity": None,
    "Brightness": 0,
    "Status": False,
}
state_lock = threading.Lock()
ser = None
serial_lock = threading.Lock()

gui_queue = queue.Queue()

chat_history = []
chat_lock = threading.Lock()
MAX_HISTORY = 20


def log(msg: str):
    print(f"[{time.strftime('%H:%M:%S')}] {msg}")


# ==================== 3. 串口协议 ====================
def build_light_frame(brightness: int) -> bytes:
    brightness = max(0, min(100, brightness))
    bai = (brightness // 100) % 10
    shi = (brightness // 10) % 10
    ge = brightness % 10
    frame = bytearray([0xAA, 0xAA, 0x02, 0x03, bai, shi, ge])
    frame.append(0x02 ^ 0x03 ^ bai ^ shi ^ ge)
    frame.append(0xBB)
    return bytes(frame)


def parse_ht_frame(buf: bytes):
    if len(buf) < 13 or buf[0] != 0xAA or buf[1] != 0xAA:
        return None
    if buf[2] != 0x01 or buf[3] != 0x07 or buf[12] != 0xBB:
        return None
    checksum = 0x01 ^ 0x07
    for i in range(7):
        checksum ^= buf[4 + i]
    if checksum != buf[11]:
        return None
    sign = buf[4]
    temperature = buf[5] * 10 + buf[6] + buf[7] * 0.1
    if sign == 0x02:
        temperature = -temperature
    humidity = buf[8] * 100 + buf[9] * 10 + buf[10]
    return temperature, humidity


serial_buffer = bytearray()


def serial_open():
    global ser
    while True:
        try:
            ser = serial.Serial(SERIAL_PORT, SERIAL_BAUD, timeout=SERIAL_TIMEOUT)
            log(f"✅ 串口已打开: {SERIAL_PORT} @ {SERIAL_BAUD}")
            return
        except Exception as e:
            log(f"❌ 打开串口失败: {e}，2 秒后重试")
            time.sleep(2)


_last_alarm = {}


def check_alarm(temperature, humidity):
    now = time.time()
    msg = None

    if temperature is not None:
        if temperature > TEMP_HIGH:
            key = "T_HIGH"
            if now - _last_alarm.get(key, 0) > ALARM_INTERVAL:
                _last_alarm[key] = now
                msg = f"警告，温度过高，当前{temperature:.1f}摄氏度"
        elif temperature < TEMP_LOW:
            key = "T_LOW"
            if now - _last_alarm.get(key, 0) > ALARM_INTERVAL:
                _last_alarm[key] = now
                msg = f"警告，温度过低，当前{temperature:.1f}摄氏度"

    if msg is None and humidity is not None:
        if humidity > HUM_HIGH:
            key = "H_HIGH"
            if now - _last_alarm.get(key, 0) > ALARM_INTERVAL:
                _last_alarm[key] = now
                msg = f"警告，湿度过高，当前湿度百分之{humidity}"
        elif humidity < HUM_LOW:
            key = "H_LOW"
            if now - _last_alarm.get(key, 0) > ALARM_INTERVAL:
                _last_alarm[key] = now
                msg = f"警告，湿度过低，当前湿度百分之{humidity}"

    if msg:
        log(f"🔔 告警: {msg}")
        speak(msg)
        gui_queue.put(("append_chat", f"🔔 系统告警：{msg}\n"))


def serial_reader():
    global serial_buffer
    while True:
        try:
            data = ser.read(256)
            if not data:
                continue
            serial_buffer.extend(data)
            while True:
                if len(serial_buffer) < 13:
                    break
                if serial_buffer[0] != 0xAA or serial_buffer[1] != 0xAA:
                    serial_buffer.pop(0)
                    continue
                frame = bytes(serial_buffer[:13])
                result = parse_ht_frame(frame)
                if result:
                    temperature, humidity = result
                    with state_lock:
                        state["Temperature"] = temperature
                        state["Humidity"] = humidity
                    log(f"📥 串口: T={temperature:.1f}℃, H={humidity}%")
                    report_to_cloud()
                    gui_queue.put(("refresh_state", None))
                    check_alarm(temperature, humidity)
                    del serial_buffer[:13]
                else:
                    serial_buffer.pop(0)
        except Exception as e:
            log(f"❌ 串口读取异常: {e}")
            time.sleep(0.5)


def send_light_to_board(brightness: int):
    frame = build_light_frame(brightness)
    with serial_lock:
        if ser and ser.is_open:
            ser.write(frame)
            log(f"📤 下发亮度: {brightness}")


# ==================== 4. MQTT ====================
client = mqtt.Client(
    mqtt.CallbackAPIVersion.VERSION2,
    client_id=device_name
)
client.username_pw_set(product_id, token)


def report_to_cloud(verbose=False):
    """拼写正确：Brightness / value"""
    with state_lock:
        params = {}
        if state["Temperature"] is not None:
            params["Temperature"] = {"value": round(float(state["Temperature"]), 1)}
        if state["Humidity"] is not None:
            params["Humidity"] = {"value": int(state["Humidity"])}
        if state["Brightness"] is not None:
            params["Brightness"] = {"value": int(state["Brightness"])}
        if state["Status"] is not None:
            params["Status"] = {"value": bool(state["Status"])}
        if not params:
            return
        payload = {
            "id": str(int(time.time() * 1000)),
            "version": "1.0",
            "params": params
        }
    result = client.publish(pub_topic, json.dumps(payload), qos=1)
    if result.rc == mqtt.MQTT_ERR_SUCCESS and verbose:
        log(f"☁️ 上报: {params}")
    elif result.rc != mqtt.MQTT_ERR_SUCCESS:
        log(f"❌ 上报失败: rc={result.rc}")


def reply_property_set(request_id):
    payload = {"id": request_id, "code": 200, "msg": "success"}
    client.publish(set_reply_topic, json.dumps(payload), qos=1)


def on_connect(client, userdata, flags, reason_code, properties):
    if reason_code.is_failure:
        log(f"❌ OneNET 连接失败: {reason_code}")
        return
    log("✅ OneNET 连接成功")
    client.subscribe(sub_topic, qos=1)
    log(f"📥 已订阅: {sub_topic}")


def on_message(client, userdata, msg):
    try:
        data = json.loads(msg.payload.decode())
        request_id = data.get("id", str(int(time.time() * 1000)))
        params = data.get("params", {})
        log(f"☁️ 云端下发: {params}")

        with state_lock:
            if "Status" in params:
                raw = params["Status"]
                v = raw["value"] if isinstance(raw, dict) else raw
                state["Status"] = bool(v)
                state["Brightness"] = 0
                send_light_to_board(0)

            if "Brightness" in params:
                raw = params["Brightness"]
                v = int(raw["value"] if isinstance(raw, dict) else raw)
                if state["Status"]:
                    state["Brightness"] = v
                    send_light_to_board(v)

            if "Temperature" in params:
                raw = params["Temperature"]
                state["Temperature"] = float(raw["value"] if isinstance(raw, dict) else raw)
            if "Humidity" in params:
                raw = params["Humidity"]
                state["Humidity"] = int(raw["value"] if isinstance(raw, dict) else raw)

        reply_property_set(request_id)
        report_to_cloud(verbose=True)
        gui_queue.put(("refresh_state", None))
    except Exception as e:
        log(f"❌ 处理云端消息失败: {e}")


client.on_connect = on_connect
client.on_message = on_message


# ==================== 5. DeepSeek ====================
llm = OpenAI(api_key=DEEPSEEK_KEY, base_url="https://api.deepseek.com")

SYSTEM_PROMPT = """你是一个智能家居控制助手，负责控制灯光和查询温湿度。

【记忆能力】
你会记住本次会话中用户说过的所有信息（姓名、喜好、习惯等）。
当用户问“我是谁”“我刚才说了什么”“我喜欢什么”等问题时，请根据对话历史回答。
如果历史里确实没有相关信息，才说“你还没告诉我”。

【设备说明】
- 灯 Status（开关 true/false）、Brightness（亮度 0~100 整数）
- 传感器 Temperature（温度 ℃）、Humidity（湿度 %）

【输出要求 - 极其重要】
每次回复必须输出一个完整 JSON 对象，包含 reply 和 cmd 两个字段！
- reply：给用户的中文口语回复
- cmd：控制指令，即使不控制设备也必须写成空对象 {}

✅ 正确格式：{"reply": "好的，灯已打开", "cmd": {"Status": true}}
✅ 正确格式：{"reply": "当前温度25.3摄氏度", "cmd": {}}
❌ 错误格式：{"reply": "好的，灯已打开"}         ← 缺 cmd
❌ 错误格式：好的，灯已打开                       ← 不是 JSON

规则：
1. “开灯” → {"Status": true}
2. “关灯” → {"Status": false}
3. “亮度50” → {"Status": true, "Brightness": 50}
4. “调暗一点” → 基于当前亮度减少约 30
5. 查询温度/湿度 → cmd 为 {}，reply 用口语化方式播报数值
6. 身份/记忆类问题 → cmd 为 {}，根据对话历史回答
7. 无法识别 → cmd 为 {}，reply 礼貌说明

【当前设备状态】
__SENSOR_STATE__

【示例】
用户：我叫小明
输出：{"reply": "好的小明，记住你了", "cmd": {}}
用户：我是谁
输出：{"reply": "你是小明呀", "cmd": {}}
用户：把灯打开
输出：{"reply": "好的小明，灯已打开", "cmd": {"Status": true}}
用户：亮度调到60
输出：{"reply": "好的，亮度已调到60", "cmd": {"Status": true, "Brightness": 60}}
用户：现在温度多少
输出：{"reply": "现在室内温度是25.3摄氏度", "cmd": {}}
用户：屋里热不热
输出：{"reply": "现在室内温度25.3摄氏度，比较舒适", "cmd": {}}
"""


def clean(text: str) -> str:
    if not text:
        return ""
    for t in ["<|end|>", "<|endoftext|>", "<|eot|>"]:
        text = text.replace(t, "")
    return text.strip()


def extract_json(text: str):
    text = clean(text)
    text = re.sub(r"^```(?:json)?", "", text.strip())
    text = re.sub(r"```$", "", text.strip()).strip()
    try:
        return json.loads(text)
    except Exception:
        pass
    start, end = text.find("{"), text.rfind("}")
    if start != -1 and end != -1 and end > start:
        try:
            return json.loads(text[start:end + 1])
        except Exception:
            pass
    return None


def infer_cmd_from_text(text: str) -> dict:
    """关键词兜底：从文本推断控制指令"""
    cmd = {}

    has_close = bool(re.search(r"(关闭|关掉|把灯关|关灯|熄灭|灭了)", text))
    has_open = bool(re.search(r"(打开|开一下|把灯开|开灯|点亮|亮起来)", text))

    if has_open and not has_close:
        cmd["Status"] = True
    elif has_close and not has_open:
        cmd["Status"] = False
    elif has_open and has_close:
        pos_open = max(text.rfind("开"), text.rfind("打"))
        pos_close = max(text.rfind("关"), text.rfind("灭"))
        cmd["Status"] = pos_open > pos_close

    m = re.search(r"(?:亮度|调到|设成|设为)\s*(\d{1,3})", text)
    if m:
        b = int(m.group(1))
        if 0 <= b <= 100:
            cmd["Brightness"] = b
            if "Status" not in cmd:
                cmd["Status"] = True

    return cmd


def parse_llm_output(content: str) -> dict:
    content = clean(content)
    parsed = extract_json(content)

    if parsed and isinstance(parsed, dict):
        reply = clean(str(parsed.get("reply", "好的")))
        cmd = parsed.get("cmd", {})
        if not isinstance(cmd, dict):
            cmd = {}

        if not cmd:
            inferred = infer_cmd_from_text(reply)
            if inferred:
                log(f"🔧 模型未给出 cmd，已根据关键词推断: {inferred}")
                cmd = inferred

        return {"reply": reply, "cmd": cmd}

    text = content
    cmd = infer_cmd_from_text(text)
    if not cmd:
        log(f"⚠️ 模型输出非 JSON，已按纯文本处理: {text!r}")

    return {"reply": text if text else "好的", "cmd": cmd}


def ask_llm(user_text: str) -> dict:
    with state_lock:
        t = state["Temperature"]
        h = state["Humidity"]
        sensor_state = (
            f"- 温度: {'未知（传感器未上报）' if t is None else f'{t:.1f}℃'}\n"
            f"- 湿度: {'未知（传感器未上报）' if h is None else f'{h}%'}\n"
            f"- 灯开关: {'开' if state['Status'] else '关'}\n"
            f"- 灯亮度: {state['Brightness']}"
        )

    with chat_lock:
        messages = [
            {"role": "system",
             "content": SYSTEM_PROMPT.replace("__SENSOR_STATE__", sensor_state)}
        ]
        messages.extend(chat_history)
        messages.append({"role": "user", "content": user_text})

    try:
        resp = llm.chat.completions.create(
            model="deepseek-chat",
            messages=messages,
            stream=False,
            temperature=0.3,
        )
        content = resp.choices[0].message.content
    except Exception as e:
        log(f"[LLM 请求出错] {e}")
        return {"reply": "抱歉，网络出了点问题。", "cmd": {}}

    result = parse_llm_output(content)
    reply = result["reply"]
    cmd = result["cmd"]
    log(f"📝 解析结果: reply={reply!r}, cmd={cmd}")

    with chat_lock:
        chat_history.append({"role": "user", "content": user_text})
        chat_history.append({
            "role": "assistant",
            "content": json.dumps({"reply": reply, "cmd": cmd}, ensure_ascii=False)
        })
        while len(chat_history) > MAX_HISTORY:
            chat_history.pop(0)

    return {"reply": reply, "cmd": cmd}


# ==================== 6. 指令执行 ====================
def execute_cmd(cmd: dict):
    if not cmd:
        log("⏸ cmd 为空，不执行硬件操作")
        return
    log(f"⚙ 执行指令: {cmd}")
    with state_lock:
        new_status = state["Status"]
        new_bright = state["Brightness"]

        if "Status" in cmd:
            new_status = bool(cmd["Status"])
            if new_status:
                if "Brightness" not in cmd and new_bright in (None, 0):
                    new_bright = 50
            else:
                new_bright = 0

        if "Brightness" in cmd and new_status:
            new_bright = max(0, min(100, int(cmd["Brightness"])))

        state["Status"] = new_status
        state["Brightness"] = new_bright
        send_light_to_board(new_bright)

    report_to_cloud(verbose=True)
    gui_queue.put(("refresh_state", None))


# ==================== 7. TTS 语音播报 ====================
# ★ 关键修复：每次播报都新建一个 pyttsx3 engine，播完立即销毁。
#   原因：pyttsx3 在同一个 engine 上连续调用 runAndWait()，
#   第二次及以后会静默失败（毫秒级返回，不发声）。
#   这是 pyttsx3 的知名 bug，官方仓库 #210 / #196 / #166 都有记录。
_tts_queue = queue.Queue()
_tts_worker_started = False
_tts_worker_lock = threading.Lock()


def _create_engine():
    """创建一个配置好的 pyttsx3 engine"""
    engine = pyttsx3.init()
    engine.setProperty("rate", 180)
    engine.setProperty("volume", 0.9)
    for v in engine.getProperty("voices"):
        if any(k in v.name for k in ("Huihui", "Yaoyao", "Kangkang", "Chinese", "中文")):
            engine.setProperty("voice", v.id)
            break
    return engine


def _tts_worker():
    """TTS 工作线程：串行处理队列，每条消息用全新 engine"""
    while True:
        try:
            text = _tts_queue.get()
            if text is None:
                break

            engine = None
            try:
                t0 = time.time()
                log(f"🔊 正在播报: {text[:30]}")

                engine = _create_engine()
                engine.say(text)
                engine.runAndWait()

                elapsed = time.time() - t0
                log(f"🔊 播报完成 (耗时 {elapsed:.2f}s): {text[:30]}")
                if elapsed < 0.3:
                    log("⚠️ 播报耗时过短，可能未真正发声。")
            except Exception as e:
                log(f"[语音播放失败] {e}")
            finally:
                if engine is not None:
                    try:
                        engine.stop()
                    except Exception:
                        pass
                    del engine
                _tts_queue.task_done()
        except Exception as e:
            log(f"[TTS worker 异常] {e}")


def _ensure_tts_worker():
    global _tts_worker_started
    with _tts_worker_lock:
        if not _tts_worker_started:
            t = threading.Thread(target=_tts_worker, daemon=True)
            t.start()
            _tts_worker_started = True


def tts_normalize(text: str) -> str:
    if not text:
        return ""
    text = text.replace("℃", "摄氏度")
    text = re.sub(r"(\d+(?:\.\d+)?)\s*%", r"百分之\1", text)
    text = re.sub(r"\s+", " ", text).strip()
    return text


def speak(text):
    """把文本放进 TTS 队列（非阻塞）"""
    if not text or not text.strip():
        return
    text = tts_normalize(text)
    _ensure_tts_worker()
    _tts_queue.put(text)
    log(f"🔊 已加入语音队列: {text[:30]}")


# ==================== 7.7 离线语音识别（SenseVoice） ====================
_sherpa_onnx = None
_asr_recognizer = None
_asr_lock = threading.Lock()


def load_asr_recognizer():
    global _sherpa_onnx, _asr_recognizer
    if _asr_recognizer is not None:
        return _asr_recognizer
    with _asr_lock:
        if _asr_recognizer is not None:
            return _asr_recognizer
        import sherpa_onnx as so
        _sherpa_onnx = so

        model_path = os.path.join(MODEL_DIR, "model.int8.onnx")
        tokens_path = os.path.join(MODEL_DIR, "tokens.txt")

        if not os.path.exists(model_path):
            raise FileNotFoundError(f"找不到模型: {model_path}")
        if not os.path.exists(tokens_path):
            raise FileNotFoundError(f"找不到 tokens: {tokens_path}")

        log("🔄 正在加载 SenseVoice 模型...")
        _asr_recognizer = so.OfflineRecognizer.from_sense_voice(
            model=model_path,
            tokens=tokens_path,
            use_itn=True,
            language="auto",
            num_threads=2,
            debug=False,
        )
        log("✅ SenseVoice 模型加载完成")
        return _asr_recognizer


def record_until_event(stop_event: threading.Event,
                        max_seconds=MAX_RECORD_SECONDS,
                        silence_duration=SILENCE_DURATION,
                        silence_threshold=SILENCE_THRESHOLD):
    chunk_seconds = BLOCK_SIZE / ASR_RATE
    silent_chunks_needed = int(silence_duration / chunk_seconds)
    max_chunks = int(max_seconds / chunk_seconds)

    buffer = []
    silent_count = 0
    has_voice = False

    try:
        with sd.InputStream(
            samplerate=ASR_RATE,
            channels=ASR_CHANNELS,
            dtype=ASR_DTYPE,
            blocksize=BLOCK_SIZE,
        ) as stream:
            for _ in range(max_chunks):
                if stop_event.is_set():
                    log("🛑 用户点击'我说完了'，停止录音")
                    break

                data, overflowed = stream.read(BLOCK_SIZE)
                if overflowed:
                    log("⚠️ 录音溢出")

                samples = data.flatten().astype(np.int16)
                buffer.append(samples)

                max_amp = int(np.abs(samples).max())
                if max_amp > silence_threshold:
                    has_voice = True
                    silent_count = 0
                else:
                    if has_voice:
                        silent_count += 1
                        if silent_count >= silent_chunks_needed:
                            log(f"🛑 静音 {silence_duration}s 兜底停止录音")
                            break

        if not buffer:
            return np.array([], dtype=np.int16)

        audio = np.concatenate(buffer, axis=0).astype(np.int16)
        duration = len(audio) / ASR_RATE
        log(f"🎙️ 录音结束，时长 {duration:.2f} 秒")

        if not has_voice:
            log("⚠️ 未检测到人声")
            return np.array([], dtype=np.int16)

        return audio

    except Exception as e:
        log(f"❌ 录音失败: {e}")
        return np.array([], dtype=np.int16)


def transcribe(audio: np.ndarray) -> str:
    if audio is None or len(audio) == 0:
        return ""
    try:
        recognizer = load_asr_recognizer()
        stream = recognizer.create_stream()
        stream.accept_waveform(ASR_RATE, audio)
        recognizer.decode_stream(stream)
        return stream.result.text.strip()
    except Exception as e:
        log(f"❌ 识别失败: {e}")
        return ""


# ==================== 8. GUI ====================
class App:
    def __init__(self, root: tk.Tk):
        self.root = root
        root.title("智能家居控制面板")
        root.geometry("1040x660")
        root.minsize(960, 620)

        self._voice_busy = False
        self._stop_event = None

        style = ttk.Style()
        try:
            style.theme_use("clam")
        except Exception:
            pass

        # ---------- 左侧 ----------
        left = ttk.Frame(root, padding=10)
        left.pack(side=tk.LEFT, fill=tk.Y)

        ttk.Label(left, text="📊 设备状态", font=("Microsoft YaHei", 14, "bold")) \
            .pack(anchor="w", pady=(0, 8))

        self.lbl_temp = ttk.Label(left, text="温度: -- ℃", font=("Microsoft YaHei", 12))
        self.lbl_hum = ttk.Label(left, text="湿度: -- %", font=("Microsoft YaHei", 12))
        self.lbl_status = ttk.Label(left, text="灯: 关", font=("Microsoft YaHei", 12))
        self.lbl_bright = ttk.Label(left, text="亮度: 0", font=("Microsoft YaHei", 12))
        for w in (self.lbl_temp, self.lbl_hum, self.lbl_status, self.lbl_bright):
            w.pack(anchor="w", pady=2)

        ttk.Separator(left, orient="horizontal").pack(fill=tk.X, pady=10)

        ttk.Label(left, text="🎛 手动控制", font=("Microsoft YaHei", 14, "bold")) \
            .pack(anchor="w", pady=(0, 8))

        btn_row = ttk.Frame(left)
        btn_row.pack(anchor="w", pady=4)
        ttk.Button(btn_row, text="开灯", width=8, command=self.on_light_on).pack(side=tk.LEFT, padx=2)
        ttk.Button(btn_row, text="关灯", width=8, command=self.on_light_off).pack(side=tk.LEFT, padx=2)

        ttk.Label(left, text="亮度调节 (0~100)", font=("Microsoft YaHei", 11)) \
            .pack(anchor="w", pady=(10, 2))

        self.bright_var = tk.IntVar(value=0)
        self.scale = ttk.Scale(
            left, from_=0, to=100, orient=tk.HORIZONTAL,
            length=220, variable=self.bright_var,
            command=self.on_scale_move
        )
        self.scale.pack(anchor="w")

        self.lbl_scale_val = ttk.Label(left, text="当前滑块: 0", font=("Microsoft YaHei", 10))
        self.lbl_scale_val.pack(anchor="w")

        ttk.Button(left, text="应用亮度", width=14, command=self.on_apply_bright) \
            .pack(anchor="w", pady=8)

        ttk.Separator(left, orient="horizontal").pack(fill=tk.X, pady=10)

        ttk.Label(left, text="⚙ 选项", font=("Microsoft YaHei", 12, "bold")) \
            .pack(anchor="w", pady=(0, 4))

        self.var_voice = tk.BooleanVar(value=True)
        ttk.Checkbutton(left, text="语音播报", variable=self.var_voice).pack(anchor="w")

        # ---------- 右侧 ----------
        right = ttk.Frame(root, padding=10)
        right.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)

        ttk.Label(right, text="💬 与大模型对话", font=("Microsoft YaHei", 14, "bold")) \
            .pack(anchor="w", pady=(0, 6))

        self.chat = scrolledtext.ScrolledText(
            right, wrap=tk.WORD, font=("Microsoft YaHei", 11),
            state=tk.DISABLED, height=20
        )
        self.chat.pack(fill=tk.BOTH, expand=True)
        self.chat.tag_config("user", foreground="#0a5")
        self.chat.tag_config("assistant", foreground="#04a")
        self.chat.tag_config("sys", foreground="#888")
        self.chat.tag_config("alarm", foreground="#c00")

        # ============ 语音交互区 ============
        voice_row = ttk.Frame(right)
        voice_row.pack(fill=tk.X, pady=(8, 4))

        self.btn_voice_input = tk.Button(
            voice_row,
            text="🎤 点击开始语音交互（F2）",
            font=("Microsoft YaHei", 12, "bold"),
            bg="#27ae60", fg="white",
            activebackground="#1e8449", activeforeground="white",
            relief="flat", cursor="hand2", height=2,
            command=self.on_voice_start
        )
        self.btn_voice_input.pack(side=tk.LEFT, fill=tk.X, expand=True, padx=(0, 4))

        self.btn_voice_done = tk.Button(
            voice_row,
            text="✅ 我说完了",
            font=("Microsoft YaHei", 12, "bold"),
            bg="#bdc3c7", fg="white",
            activebackground="#95a5a6", activeforeground="white",
            relief="flat", cursor="hand2", height=2, width=14,
            state=tk.DISABLED,
            command=self.on_voice_done
        )
        self.btn_voice_done.pack(side=tk.LEFT)

        # 输入行
        input_row = ttk.Frame(right)
        input_row.pack(fill=tk.X, pady=(4, 0))

        self.entry = ttk.Entry(input_row, font=("Microsoft YaHei", 11))
        self.entry.pack(side=tk.LEFT, fill=tk.X, expand=True, padx=(0, 6))
        self.entry.bind("<Return>", lambda e: self.on_send())

        self.btn_send = ttk.Button(input_row, text="发送", width=10, command=self.on_send)
        self.btn_send.pack(side=tk.LEFT)

        # 快捷指令
        quick1 = ttk.Frame(right)
        quick1.pack(fill=tk.X, pady=(6, 0))
        ttk.Label(quick1, text="快捷：", font=("Microsoft YaHei", 10)).pack(side=tk.LEFT)
        for text in ["把灯打开", "把灯关掉", "亮度调到 60"]:
            ttk.Button(quick1, text=text, command=lambda t=text: self.quick_send(t)) \
                .pack(side=tk.LEFT, padx=2)
        ttk.Button(quick1, text="清空对话", command=self.on_clear_chat) \
            .pack(side=tk.RIGHT, padx=2)

        quick2 = ttk.Frame(right)
        quick2.pack(fill=tk.X, pady=(4, 0))
        ttk.Label(quick2, text="　　　", font=("Microsoft YaHei", 10)).pack(side=tk.LEFT)
        for text in ["现在温度多少", "现在湿度多少", "温湿度都报一下"]:
            ttk.Button(quick2, text=text, command=lambda t=text: self.quick_send(t)) \
                .pack(side=tk.LEFT, padx=2)

        root.bind("<F2>", lambda e: self.on_voice_start())
        root.bind("<F3>", lambda e: self.on_voice_done())

        self.append_chat("sys",
                         "面板已就绪~\n"
                         "点「🎤 开始语音交互」（F2）→ 说话 → 点「✅ 我说完了」（F3）\n"
                         "也可以打字：开灯 / 关灯 / 亮度调到 60 / 现在温度多少\n\n")

    def append_chat(self, tag: str, text: str):
        self.chat.config(state=tk.NORMAL)
        self.chat.insert(tk.END, text, tag)
        self.chat.see(tk.END)
        self.chat.config(state=tk.DISABLED)

    def quick_send(self, text):
        self.entry.delete(0, tk.END)
        self.entry.insert(0, text)
        self.on_send()

    def on_send(self):
        text = self.entry.get().strip()
        if not text:
            return
        self.entry.delete(0, tk.END)
        self.append_chat("user", f"你：{text}\n")
        log(f"👤 用户: {text}")

        voice_enabled = bool(self.var_voice.get())

        self.btn_send.config(state=tk.DISABLED)
        threading.Thread(
            target=self._ask_worker,
            args=(text, voice_enabled),
            daemon=True
        ).start()

    def _ask_worker(self, text, voice_enabled):
        result = ask_llm(text)
        reply = result["reply"]
        cmd = result["cmd"]
        log(f"🤖 助手: {reply} | cmd={cmd}")

        if cmd:
            try:
                execute_cmd(cmd)
            except Exception as e:
                log(f"❌ 执行指令失败: {e}")

        if voice_enabled:
            speak(reply)

        self.root.after(0, self._after_reply, reply, cmd)

    def _after_reply(self, reply, cmd):
        self.append_chat("assistant", f"助手：{reply}\n")
        if cmd:
            self.append_chat("sys", f"    [指令] {cmd}\n")
        self.append_chat("sys", "\n")
        self.btn_send.config(state=tk.NORMAL)

    def on_voice_start(self):
        if self._voice_busy:
            log("⚠️ 语音交互进行中")
            return
        self._voice_busy = True
        self._stop_event = threading.Event()

        self.btn_voice_input.config(
            state=tk.DISABLED, text="🎤 正在录音...", bg="#e67e22"
        )
        self.btn_voice_done.config(
            state=tk.NORMAL, bg="#e74c3c",
            text="✅ 我说完了", cursor="hand2"
        )
        self.btn_send.config(state=tk.DISABLED)
        self.append_chat("sys", "🎤 正在录音，说完请点「✅ 我说完了」（或按 F3）...\n")

        def worker():
            try:
                audio = record_until_event(self._stop_event)
                if len(audio) == 0:
                    self.root.after(0, self._after_voice_done, "")
                    return
                log("🔄 识别中...")
                text = transcribe(audio)
                del audio
                if text:
                    log(f"🎯 识别结果: {text}")
                else:
                    log("❓ 未能识别出内容")
                self.root.after(0, self._after_voice_done, text)
            except Exception as e:
                log(f"❌ 语音交互出错: {e}")
                self.root.after(0, self._after_voice_done, "")

        threading.Thread(target=worker, daemon=True).start()

    def on_voice_done(self):
        if not self._voice_busy or self._stop_event is None:
            return
        if self._stop_event.is_set():
            return
        self._stop_event.set()
        self.btn_voice_done.config(state=tk.DISABLED, bg="#bdc3c7", text="⏳ 处理中...")

    def _after_voice_done(self, text):
        self._voice_busy = False
        self._stop_event = None

        self.btn_voice_input.config(
            state=tk.NORMAL, text="🎤 点击开始语音交互（F2）", bg="#27ae60"
        )
        self.btn_voice_done.config(
            state=tk.DISABLED, bg="#bdc3c7", text="✅ 我说完了"
        )
        self.btn_send.config(state=tk.NORMAL)

        if not text:
            self.append_chat("sys", "（未识别到内容，请重试）\n\n")
            return

        self.entry.delete(0, tk.END)
        self.entry.insert(0, text)
        self.on_send()

    def on_clear_chat(self):
        with chat_lock:
            chat_history.clear()
        self.chat.config(state=tk.NORMAL)
        self.chat.delete("1.0", tk.END)
        self.chat.config(state=tk.DISABLED)
        self.append_chat("sys", "🧹 对话历史已清空，模型不再记得之前的内容\n\n")
        log("🧹 对话历史已清空")

    def on_light_on(self):
        with state_lock:
            state["Status"] = True
            if state["Brightness"] in (None, 0):
                state["Brightness"] = 50
            b = state["Brightness"]
            send_light_to_board(b)
        report_to_cloud(verbose=True)
        self.refresh_state()
        self.append_chat("sys", f"🖱 手动开灯，亮度={b}\n\n")

    def on_light_off(self):
        with state_lock:
            state["Status"] = False
            state["Brightness"] = 0
            send_light_to_board(0)
        report_to_cloud(verbose=True)
        self.refresh_state()
        self.append_chat("sys", "🖱 手动关灯\n\n")

    def on_scale_move(self, _):
        self.lbl_scale_val.config(text=f"当前滑块: {int(float(self.bright_var.get()))}")

    def on_apply_bright(self):
        b = int(float(self.bright_var.get()))
        with state_lock:
            if not state["Status"]:
                state["Status"] = True
            state["Brightness"] = b
            send_light_to_board(b)
        report_to_cloud(verbose=True)
        self.refresh_state()
        self.append_chat("sys", f"🖱 手动设置亮度 = {b}\n\n")

    def refresh_state(self):
        with state_lock:
            t = state["Temperature"]
            h = state["Humidity"]
            s = state["Status"]
            b = state["Brightness"]

        self.lbl_temp.config(text=f"温度: {'--' if t is None else f'{t:.1f} ℃'}")
        self.lbl_hum.config(text=f"湿度: {'--' if h is None else f'{h} %'}")
        self.lbl_status.config(text=f"灯: {'开' if s else '关'}")
        self.lbl_bright.config(text=f"亮度: {b if b is not None else 0}")

    def poll_queue(self):
        try:
            while True:
                kind, payload = gui_queue.get_nowait()
                if kind == "refresh_state":
                    self.refresh_state()
                elif kind == "append_chat":
                    self.append_chat("sys", payload)
        except queue.Empty:
            pass
        self.root.after(200, self.poll_queue)


# ==================== 9. 主程序 ====================
def main():
    serial_open()
    threading.Thread(target=serial_reader, daemon=True).start()
    time.sleep(0.3)
    send_light_to_board(0)

    client.connect(hostname, port, 60)
    client.loop_start()

    def periodic_report():
        while True:
            try:
                report_to_cloud()
            except Exception as e:
                log(f"❌ 上报异常: {e}")
            time.sleep(5)

    threading.Thread(target=periodic_report, daemon=True).start()

    def warmup_asr():
        try:
            load_asr_recognizer()
        except Exception as e:
            log(f"⚠️ ASR 预加载失败（点语音按钮时会重试）: {e}")

    threading.Thread(target=warmup_asr, daemon=True).start()

    root = tk.Tk()
    app = App(root)
    app.refresh_state()
    app.poll_queue()
    log("🚀 GUI 已启动，终端仅打印日志")
    root.mainloop()

    client.loop_stop()
    client.disconnect()
    if ser and ser.is_open:
        ser.close()
    log("🔌 已关闭串口和 MQTT")


if __name__ == "__main__":
    main()