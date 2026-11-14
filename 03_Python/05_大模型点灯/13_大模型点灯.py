import json
import re
import threading
from deepseek_utils import set_role, set_max_history, chat
from sherpa_utils import speech_to_text, text_to_speech

from serial import Serial
from protocol import make_light_frame, parse_temp_humi_frame

# 创建串口对象，打开串口
ser = Serial("COM35", 115200, timeout=1)

# 全局变量  温湿度
temp = 0.0
humi = 0

def read_uart_data(): # 子线程处理函数
    while True:
        data = ser.read(13)
        if not data: # 没有数据，跳过循环
            continue
        
        status, data_list = parse_temp_humi_frame(data)
        if status == 0:
            global temp
            global humi
            temp = data_list[0]
            humi = data_list[1]
            # print(temp, humi)

# 开线程，  读串口数据
threading.Thread(target=read_uart_data, daemon=True).start()

role_define = """
你是智能家居管家贾维斯，管理家中设备，也给主人一点温暖。

可用设备与函数（括号里是参数）：
- 厨房灯： set_kitchen_light(brightness)   brightness 取 0~100
- 阳台灯： set_balcony_light(brightness)   brightness 取 0~100
- 空调：   set_ac(temperature)             温度取 16~30
- 空调：   turn_off_ac()                   关空调
- 洗衣机： start_wash() / start_dry()
- 传感器： get_temperature() / get_humidity()

必须按 JSON 格式回复：
{
    "device_control": {"function": "函数名，没有设备写 null（不要写Python的None）", "params": {}, "message": "执行说明"},
    "emotion_support": {"message": "温暖回应(以。作为末尾符号)"}
}

示例1：“厨房灯调暗一点”
{"device_control": {"function": "set_kitchen_light", "params": {"brightness": 20}, "message": "厨房灯调到20%"}, "emotion_support": {"message": "调暗啦。"}}

示例2：“现在几度？”
{"device_control": {"function": "get_temperature", "params": {}, "message": "读取温度"}, "emotion_support": {"message": "情绪价值(以。作为末尾符号)"}}
"""

# ========== 小工具：把数字夹到范围内 ==========
def clamp(value, low, high):
    # 将value限制在[low, high]区间内，防止设备参数越界
    if value < low:
        value = low
    if value > high:
        value = high
    return value

# ========== 设备函数（模拟硬件操作） ==========
def set_kitchen_light(brightness=100):
    brightness = clamp(brightness, 0, 100)  # 亮度钳制到0~100
    print("【执行】厨房灯亮度", brightness, "%")
    ser.write(make_light_frame(brightness))

def set_balcony_light(brightness=100):
    brightness = clamp(brightness, 0, 100)  # 亮度钳制到0~100
    print("【执行】阳台灯亮度", brightness, "%")
    ser.write(make_light_frame(brightness))

def set_ac(temperature=26):
    temperature = clamp(temperature, 16, 30)  # 温度钳制到16~30℃
    print("【执行】空调已开启，温度", temperature, "℃")

def turn_off_ac():
    print("【执行】空调已关闭")

def start_wash():
    print("【执行】洗衣机已启动")

def start_dry():
    print("【执行】烘干已启动")

def get_temperature():
    msg = f"当前温度为 {temp} 度"
    print(msg)     # 接上 DHT11 后换成真实读取
    text_to_speech(msg)

def get_humidity():
    msg = f"当前湿度为 {humi} "
    print(msg)       # 接上 DHT11 后换成真实读取
    text_to_speech(msg)

# ========== 白名单字典：名字 → 函数（防止AI返回任意函数名导致安全风险） ==========
DEVICES = {
    "set_kitchen_light": set_kitchen_light,
    "set_balcony_light": set_balcony_light,
    "set_ac":            set_ac,
    "turn_off_ac":       turn_off_ac,
    "start_wash":        start_wash,
    "start_dry":         start_dry,
    "get_temperature":   get_temperature,
    "get_humidity":      get_humidity,
}

def parse_result(result):
    """清洗并解析AI返回的JSON：模型可能把None写进JSON，需先转成null"""
    result = re.sub(r":\s*None\b", ": null", result)
    return json.loads(result)


def exec_device(func, params):
    """根据解析出的函数名和参数，调用真实设备函数"""
    if func is None or func == "None":
        print("无需执行设备操作")
    else:
        # 调用函数
        DEVICES[func](**params)

if __name__ == '__main__':
    # 给AI设定角色
    set_role(role_define)
    # 只保留最近 5 轮
    set_max_history(5) 

    while True:
        input("按下回车录音")
        # 录音后 语音识别
        msg = speech_to_text(duration=5)
        print("你说的是：", msg)
        
        # 调用大模型  获取推理结果
        result = chat(msg)      
        # print(result, type(result))

        # 1. 清洗后 把 JSON 字符串 解析成 Python 字典
        data = parse_result(result)

        # 2. 提取关键字段
        func    = data["device_control"]["function"]
        params  = data["device_control"]["params"]
        message = data["device_control"]["message"]
        emotion = data["emotion_support"]["message"]
        
        # print("函数名:", func)
        # print("参数:", params)
        # print("说明:", message)
        print("情绪:", emotion)
        text_to_speech(emotion)

        # 3. 执行设备操作
        exec_device(func, params)

