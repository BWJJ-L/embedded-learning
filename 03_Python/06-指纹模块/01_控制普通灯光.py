from fp_module.driver_serial import SerialDevice

if __name__ == '__main__':
    # 打开串口
    # 创建对象， 给__init__传参
    device = SerialDevice("COM12", 57600)
    # 打开串口
    if not device.open():
        print("打开串口失败")
        exit(0)
    
    # 发送指令
    device.send_bytes(bytes([0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x07, 0x3C, 0x02, 0x00, 0x00, 0x00, 0x00, 0x46]))
    
    # 接收指令
    result = device.read_bytes()
    print("接收结果：", result.hex(" "))
    
    
    # 关闭串口
    device.close()
