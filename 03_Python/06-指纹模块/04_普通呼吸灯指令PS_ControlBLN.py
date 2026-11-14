from fp_module.driver_serial import *
from fp_module.fingerprint_protocol import *

device = None  # 全局变量，方便调试
protocol = None

# 普通呼吸灯指令
def  PS_ControlBLN():
    # 发送指令
    # [0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x07,0x3C,0x02,0x04,0x04,0x00,0x00,0x4E]
    # 附加参数   0x02,0x04,0x04,0x00
    cmd = protocol.build_cmd(0x3C, bytes([0x02,0x04,0x04,0x00]))
    # print("cmd = ", cmd.hex(" "))
    device.send_bytes(cmd)
    # 接收指令，解析关键信息
    result = device.read_bytes()
    # print("result = ", result.hex(" "))
    # 通过工具解析
    data = protocol.parse_packet(result)
    # print("data = ", data)
    if data.get("status") == "ok" and data.get("confirm_code") == 0:
        print("灯光操作成功")
    else:
        print("灯光操作失败")

if __name__ == '__main__':
    # 创建对象， 给__init__传参
    device = SerialDevice("COM12", 57600)
    protocol = FingerprintProtocol()
    
    # 打开串口
    if not device.open():
        print("打开串口失败")
        exit(0)
    
    # 普通呼吸灯指令
    PS_ControlBLN()
    # 读取模组基本参数
    # PS_ReadSysPara()
    
    
    # 关闭串口
    device.close()
