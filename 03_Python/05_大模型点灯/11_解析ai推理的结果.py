import json
from deepseek_utils import set_role, set_max_history, chat

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
    "device_control": {"function": "函数名，没有设备写 None", "params": {}, "message": "执行说明"},
    "emotion_support": {"message": "温暖回应(以。作为末尾符号)"}
}

示例1：“厨房灯调暗一点”
{"device_control": {"function": "set_kitchen_light", "params": {"brightness": 20}, "message": "厨房灯调到20%"}, "emotion_support": {"message": "调暗啦。"}}

示例2：“现在几度？”
{"device_control": {"function": "get_temperature", "params": {}, "message": "读取温度"}, "emotion_support": {"message": "情绪价值(以。作为末尾符号)"}}
"""

def exec_device(func, params):
    """根据解析出的函数名和参数，调用真实设备函数"""
    if func == "None":
        print("无需执行设备操作")
    else:
        # 后续可扩展成字典映射：{"set_ac": set_ac, ...}
        print(f"调用设备函数: {func}，参数: {params}")


if __name__ == '__main__':
    # 给AI设定角色
    set_role(role_define)
    # 只保留最近 5 轮
    set_max_history(5) 

    questions = ["我有点热", "我不开心"]
    for msg in questions:
        # 调用大模型  获取推理结果
        result = chat(msg)      
        print(result, type(result))

        # 1. 把 JSON 字符串 解析成 Python 字典
        data = json.loads(result)

        # 2. 提取关键字段
        func    = data["device_control"]["function"]
        params  = data["device_control"]["params"]
        message = data["device_control"]["message"]
        emotion = data["emotion_support"]["message"]

        print("函数名:", func)
        print("参数:", params)
        print("说明:", message)
        print("情绪:", emotion)

        # 3. 执行设备操作
        exec_device(func, params)

