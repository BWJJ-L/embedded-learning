def remove_all_0x(hex_string: str) -> str:
    """
    内部辅助函数：移除字符串中所有的 0x 或 0X 前缀。
    常用于统一净化用户输入或拷贝来的 16 进制字符串。
    
    :param hex_string: 原始 16 进制字符串，如 "0x12 0XAB"
    :return: 去除前缀后的字符串，如 "12 AB"
    """
    return hex_string.replace("0x", "").replace("0X", "")


def is_hex_string(hex_string: str) -> bool:
    """
    检查输入的字符串是否是合法的 16 进制格式。
    支持带空格、带 0x 前缀的格式。
    
    :param hex_string: 待检查的字符串
    :return: bool，合法返回 True，否则返回 False
    """
    # 预处理：去除所有的 0x 前缀，并删掉所有的空格
    clean_str = remove_all_0x(hex_string).replace(" ", "")
    
    # 拦截空字符串（因为 bytes.fromhex("") 会成功返回 b''，但在业务逻辑中空串通常不算作有效的 16 进制串）
    if not clean_str:
        return False
        
    try:
        # 利用 Python 内置底层的 C 实现来做校验，比写正则表达式更快、更严谨
        bytes.fromhex(clean_str)
        return True
    except ValueError:
        return False


def hex_string_to_bytes(hex_string: str) -> bytearray:
    """
    将 16 进制格式的字符串转换为字节数组（bytearray）。
    支持如下格式（有无空格、前缀皆可）：
    "12 F2 34 72 E2" 或 "0x12 0xF2 0x34 0x72 0xE2" 或 "12F23472E2"
    
    :param hex_string: 16进制字符串
    :return: bytearray 字节数组（注意：返回的是可变序列 bytearray，而不是不可变的 bytes）
    """
    clean_str = remove_all_0x(hex_string).replace(" ", "")
    return bytearray.fromhex(clean_str)


def bytes_to_hex_string(byte_array: bytes) -> str:
    """
    将底层字节数组 (bytes/bytearray) 转换为人类可读的 16 进制字符串。
    默认用空格分割，且全部大写。
    
    :param byte_array: 原始字节流数据
    :return: 格式化后的 16 进制字符串，如 "EF 01 FF FF"
    """
    if not byte_array:
        return ""
    # Python 3.8+ bytes 对象的内置方法，直接按指定字符(空格)分割并转大写
    # 这种做法比传统的 " ".join(f"{b:02X}" for b in byte_array) 性能高很多
    return byte_array.hex(' ').upper()


def string_to_hex_string(string: str, encoding="UTF-8") -> str:
    """
    将普通文本字符串，先编码为字节流，再转换为 16 进制字符串。
    多用于需要将汉字、英文字母通过串口下发给设备的场景（如显示在设备的屏幕上）。
    
    :param string: 普通文本字符串，如 "你好" 或 "XYZ123"
    :param encoding: 字符编码格式，默认 UTF-8（有些老设备可能只支持 "gbk"）
    :return: 对应的 16 进制字符串，用空格分割
    """
    # 第一步：.encode() 转成字节流；第二步：.hex(' ') 转成带空格的16进制串
    return string.encode(encoding).hex(' ').upper()


def hex_string_to_string(hex_string: str, encoding="UTF-8") -> str:
    """
    将 16 进制字符串解码还原为普通文本字符串。
    多用于将设备上传的 16 进制数据（如设备内部的汉字名字）还原成可读文本。
    
    :param hex_string: 16 进制格式的数据串
    :param encoding: 字符解码格式，默认 UTF-8
    :return: 解码后的普通字符串
    """
    # 预处理清理格式
    clean_str = remove_all_0x(hex_string).replace(" ", "")
    # 第一步：fromhex() 还原成字节流；第二步：.decode() 按指定编码还原为字符串
    return bytes.fromhex(clean_str).decode(encoding)


if __name__ == '__main__':
    # 下方为各个工具函数的单元测试用例
    print("------------------------------16进制字符串=>字节数组------------------------------")
    print(hex_string_to_bytes("0x12 0xF2 0x34 0x72 0xE2"))
    print(hex_string_to_bytes("12 F2 34 72 E2"))

    print("------------------------------字节数组=>16进制字符串------------------------------")
    print(bytes_to_hex_string(bytes([0x12, 0xF2, 0x34, 0x72, 0xE2])))
    print(bytes_to_hex_string(b'\x12\xf24r\xe2'))
    print(bytes_to_hex_string(bytearray(b'\x12\xf24r\xe2')))

    print("-----------------------------字符串=>16进制字符串------------------------------")
    print(string_to_hex_string("XYZ123^_^", "gbk"))
    
    print("-----------------------------16进制字符串=>字符串------------------------------")
    print(hex_string_to_string("58 59 5a 31 32 33 5e 5f 5e", "gbk"))
    print(hex_string_to_string("58595a3132335e5f5e", "gbk"))

    print("-----------------------------检查是否是16进制字符串------------------------------")
    print(is_hex_string("58595a3132335e5f5e"))
    print(is_hex_string("58 59 5a 31"))
    print(is_hex_string("0x58 0x59 0x5a 0x31"))
    print(is_hex_string("av12")) # 包含非 16 进制字符 v
    print(is_hex_string("av1"))  # 长度非偶数且包含非 16 进制字符
    print(is_hex_string("噶"))    # 包含中文
    print(is_hex_string(""))     # 空串