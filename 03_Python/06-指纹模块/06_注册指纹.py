import time
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


def PS_GetEnrollImage():
    """注册用获取指纹图像
    """
    # 组包
    cmd = protocol.build_cmd(0x29)
    # 发送指令
    device.send_bytes(cmd)
    # 接收
    result = device.read_bytes()
    # 返回解析后的json
    return protocol.parse_packet(result)


def PS_GenChar(buffer_id):
    """生成特征
    Args:
        buffer_id: 存放位置  1~n顺序
    """
    # 组包
    # cmd = protocol.build_cmd(0x02, bytes([buffer_id]))
    cmd = protocol.build_cmd(0x02, struct.pack(">B", buffer_id))
    # 发送指令
    device.send_bytes(cmd)
    # 接收
    result = device.read_bytes()
    # 返回解析后的json
    return protocol.parse_packet(result)

def PS_RegModel():
    """合并特征(生成模板)
    """
    # 组包
    cmd = protocol.build_cmd(0x05)
    # 发送指令
    device.send_bytes(cmd)
    # 接收
    result = device.read_bytes()
    # 返回解析后的json
    return protocol.parse_packet(result)

def PS_StoreChar(page_id):
    """存储模板
    Args:
        page_id: 指纹库位置号
    """
    # 组包
    cmd = protocol.build_cmd(0x06, struct.pack(">BH", 1, page_id))
    # 发送指令
    device.send_bytes(cmd)
    # 接收
    result = device.read_bytes()
    # 返回解析后的json
    return protocol.parse_packet(result)

def enroll_fingerprint(page_id=0, enroll_times=3): 
    """
    通用指令注册指纹流程
    流程主要包含：注册用获取图像、生成特征、合并特征和存储模板。
    
    :param page_id: 期望存储在指纹库的 ID 号
    :param enroll_times: 录入需要按压的次数。依具体模组而定，大面积默认2次，中面积默认3次，小面积默认5次。
    """
    # 1) 循环 n 次   录入指纹  生成特征
    for n in range(1, enroll_times+1):
        print(f"\n>>> 请按压手指 (第 {n}/{enroll_times} 次) ...")
        # 死循环检测是否有手指在模块上面
        while True:
            data = PS_GetEnrollImage()
            # print("data = ", data)
            if data.get("status") == "ok":
                if data.get("confirm_code") == 0:
                    print("  ✅ 获取图像成功！")
                    break
                elif data.get("confirm_code") == 2:
                    time.sleep(0.1)
                    continue
                elif  data.get("confirm_code") == 1: # 有错
                    print(f"  ❌ 获取图像失败，错误码: {hex(data.get('confirm_code'))}")
                    return False
            else:
                print("串口通信异常")
                return False
        
        # 生成特征
        data = PS_GenChar(n)
        if data.get("status") == "ok" and data.get("confirm_code") == 0:
            print(f"  ✅ 生成特征成功！(存入 Buffer {n})")
        else:
            print(f"  ❌ 生成特征失败，错误码: {hex(data.get('confirm_code'))}")
            return False

        print("  --> 请抬起手指...")
        # 死循环检测是手指是否离开模块
        while True:
            data = PS_GetEnrollImage()
            # print("data = ", data)
            if data.get("status") == "ok":
                if data.get("confirm_code") == 0: # 手指在模块上面的
                    # print("  ✅ 获取图像成功！")
                    time.sleep(0.1)
                    continue
                elif data.get("confirm_code") == 2: # 手指不在模块上面
                    break
                elif  data.get("confirm_code") == 1: # 有错
                    print(f"  ❌ 获取图像失败，错误码: {hex(data.get('confirm_code'))}")
                    return False
            else:
                print("串口通信异常")
                return False
    
    
    
    # 2) 合并模板
    data = PS_RegModel()
    if data.get("status") == "ok" and data.get("confirm_code") == 0:
        print("  ✅ 模板合并成功！")
    else:
        print(f"  ❌ 模板合并失败，可能是几次按压偏差过大。错误码: {hex(data.get('confirm_code', 0))}")
        return False
    
    # 3) 存储
    data = PS_StoreChar(page_id)
    if data.get("status") == "ok" and data.get("confirm_code") == 0:
        print(f"🎉 恭喜！指纹注册成功！此指纹绑定的 ID 号为: {page_id}")
        return True
    else:
        print(f"  ❌ 存储模板失败，错误码: {hex(data.get('confirm_code', 0))}")
        return False

if __name__ == '__main__':
    # 创建对象， 给__init__传参
    device = SerialDevice("COM12", 57600)
    protocol = FingerprintProtocol()
    
    # 打开串口
    if not device.open():
        print("打开串口失败")
        exit(0)
    
    # 普通呼吸灯指令
    # PS_ControlBLN()
    
    # 注册指纹
    # enroll_fingerprint(0)
    enroll_fingerprint(1)
    
    # 关闭串口
    device.close()
