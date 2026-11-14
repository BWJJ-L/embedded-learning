def make_light_frame(value): # 0~100
    """
    功能：组装控制灯光协议的数据帧
    帧头        指令    长度    数据(3字节)      校验    帧尾
    0     1      2       3       4   5   6       7       8
    0xAA 0xAA   0x02   0x03    百位 十位 个位    XOR    0xBB
    # 校验 = 指令 ^ 长度 ^ 数据(n)
    """
    if not (0 <= value <= 100):
        print("灯控制值范围不对")
        return
    # value  25 ===>  0  2  5
    b100 = value // 100
    b10 = value // 10 % 10
    b1 = value % 10
    # print(b100, b10, b1)
    
    check_code = 0x02 ^ 0x03 ^ b100 ^ b10 ^ b1

    return bytes([0xAA, 0xAA, 0x02, 0x03, b100, b10, b1, check_code, 0xBB])

def parse_temp_humi_frame(data):
    """解析温湿度的协议数据帧
    Args:
        data: 原始数据
    温湿度协议帧：温度为正，符号为0x01, 温度为负0x02 
    帧头        指令    长度        数据(7字节)                        校验    帧尾
    0     1      2       3          4    5  6     7     8   9    10   11     12
    0xAA 0xAA   0x01   0x07        符号 温十 温个 温小数 湿百 湿十 湿个  XOR   0xBB
    """
    # print("data = ", data.hex(" "))  # 16进制字符串打印
    if data[0] != 0xAA or data[1] != 0xAA:
        print("帧头校验不通过")
        return -1, []
    if data[2] != 0x01:
        print("指令校验不通过")
        return -2, []
    data_frame = data[4:11]
    num_len = len(data_frame) # 自己算数据真实长度
    if data[3] != num_len:
        print("长度校验不通过")
        return -3, []
    # 计算校验
    check_code = 0
    for value in data[2:11]:
        check_code ^= value
    if data[11] != check_code:
        print("校验位校验不通过")
        return -4, []
    if data[12] != 0xBB:
        print("帧尾不通过")
        return -5, []
    
    sign, temp10, temp1, tempf, humi100, humi10, humi1 = data_frame
    # print(sign, temp10, temp1, tempf, humi100, humi10, humi1)
    temperature = temp10 * 10 + temp1 + tempf*0.1
    if sign == 0x02: # 负数
        temperature = -temperature
    humidity = humi100 * 100 + humi10 * 10 + humi1
    # print(temperature, humidity)
    return 0, [temperature, humidity]