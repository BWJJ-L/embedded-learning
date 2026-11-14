import serial
import time
from serial.tools import list_ports

class SerialDevice:
    """
    串口设备通信驱动类。
    封装了底层 pyserial 库，提供串口扫描、打开、关闭、收发数据和清空缓冲区等基础功能。
    """

    @staticmethod
    def scan_serial_ports():
        """
        扫描当前系统可用的物理/虚拟串口。
        :return: 包含可用串口信息的列表，格式为 [(串口号, 描述信息), ...]
        """
        ports = list_ports.comports()
        return [(port.device, port.description) for port in ports]

    def __init__(self, port, baud_rate=57600, timeout=1.0):
        """
        初始化串口参数。
        :param port: 串口号，例如 Windows 下的 "COM3" 或 Linux/Mac 下的 "/dev/ttyUSB0"
        :param baud_rate: 波特率，默认 57600 (通常是模块的出厂默认值)
        :param timeout: 读操作的超时时间（秒），默认 1.0 秒
        """
        self.port = port
        self.baud_rate = baud_rate
        self.timeout = timeout
        self.serial = None

    def open(self) -> bool:
        """
        打开串口连接。
        :return: bool，成功打开返回 True，否则返回 False
        """
        try:
            self.serial = serial.Serial(self.port, self.baud_rate, timeout=self.timeout)
            return self.serial.is_open
        except serial.SerialException as e:
            print(f"串口打开失败: {e}")
            return False

    def close(self):
        """
        关闭串口连接，释放系统资源。
        """
        if self.serial and self.serial.is_open:
            self.serial.close()

    def send_bytes(self, data: bytes) -> bool:
        """
        发送原始字节流数据到串口。
        :param data: 需要发送的 bytes 类型数据
        :return: bool，发送成功返回 True，否则返回 False
        """
        if not self.serial or not self.serial.is_open:
            return False
        try:
            self.serial.write(data)
            return True
        except Exception as e:
            print(f"发送数据失败: {e}")
            return False

    def read_bytes(self, wait_time=0.05) -> bytes:
        """
        阻塞读取完整的应答包数据。
        通过“捕获首字节后延时等待”的方式，一次性读取整个数据包。
        
        :param wait_time: 接收到首字节后，等待后续数据完全进入系统缓冲区的延时（秒）
        :return: 接收到的 bytes 数据，超时或出错返回空 bytes b""
        """
        if not self.serial or not self.serial.is_open:
            return b""
        try:
            # 阻塞等待第一个字节，由初始化时的 timeout 控制超时时间。
            # 这里依赖底层系统级的阻塞，不消耗 Python 层的 CPU 资源。
            first_byte = self.serial.read(1)
            if not first_byte:
                return b"" # 读取超时
                
            # 稍微延时，等待剩余的数据包完整到达串口底层缓冲区。
            # 注：57600 波特率下，传输1个字节约需 0.17ms。
            # 0.05秒(50ms)足以等待近 300 字节的数据进入缓冲区。
            time.sleep(wait_time) 
            
            # self.serial.in_waiting 可以获取当前输入缓冲区中已就绪的字节数
            # 将后续就绪的字节一次性全部读出，避免碎片化读取
            rest_bytes = self.serial.read(self.serial.in_waiting)
            return first_byte + rest_bytes
            
        except serial.SerialException as e:
            print(f"读取数据失败: {e}")
            return b""

    def flush(self):
        """
        清空串口的输入和输出缓冲区。
        通常在刚打开串口，或发送新指令前调用，用于丢弃历史遗留或乱码数据，
        防止干扰当前周期的指令通信。
        """
        if self.serial and self.serial.is_open:
            self.serial.reset_input_buffer()
            self.serial.reset_output_buffer()