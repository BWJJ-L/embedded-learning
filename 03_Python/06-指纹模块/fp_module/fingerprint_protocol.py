import struct

class FingerprintProtocol:
    """
    指纹模块通信协议类。
    负责将上层业务指令打包成底层二进制字节流，并解析底层返回的二进制数据包。
    完全遵循指纹模块手册的 "大端序、变长包、校验和" 规范。
    """
    
    # --- 包标识 (PID) 定义 ---
    PID_CMD = 0x01   # 命令包：主控发给模块的控制指令
    PID_DATA = 0x02  # 数据包：上传/下载特征或图像时的中间数据包
    PID_ACK = 0x07   # 应答包：模块回传给主控的执行结果
    PID_END = 0x08   # 结束包：上传/下载时的最后一个数据包

    def __init__(self, head=0xEF01, address=0xFFFFFFFF):
        """
        初始化协议配置。
        :param head: 包头，默认 0xEF01。
        :param address: 设备地址，默认全 0xFFFFFFFF。多设备级联时可通过指令修改。
        """
        self.head = head       # 固定包头
        self.address = address   # 固定地址

    def _build_generic_packet(self, pid: int, payload: bytes) -> bytes:
        """
        内部辅助方法：统一处理不同 PID 的数据包组装。
        手册规定：包长度 = 包长度至校验和的总字节数 (包含校验和2字节，不包含包长度自身2字节)
        """
        # 1. 计算包长度字段的值：payload长度 + 校验和(2字节)
        length = len(payload) + 2 
        
        # 2. 组装校验区数据
        # 格式: > (大端) B (PID 1字节) H (包长度 2字节)
        chk_data = struct.pack(">B H", pid, length) + payload
        
        # 3. 计算校验和
        # 手册规定：从包标识至校验和之前的所有字节之和。
        # sum() 累加所有字节，& 0xFFFF 确保结果截断为2字节（忽略溢出进位）
        checksum = sum(chk_data) & 0xFFFF
        
        # 4. 组装头部
        # 格式: > (大端) H (包头 2字节) I (地址 4字节)
        header = struct.pack(">H I", self.head, self.address)
        
        # 5. 合并并返回最终的物理数据包
        # 结构：头(6) + 校验区(3+N) + 校验和(2)
        return header + chk_data + struct.pack(">H", checksum)

    def build_cmd(self, instruction: int, params: bytes = b"") -> bytes:
        """
        组装命令包 (PID=0x01)。主控调用此方法发送控制指令。
        :param instruction: 指令码，例如 0x01(获取图像), 0x3C(控制灯)
        :param params: 指令的附加参数
        :return: 完整的命令字节流
        """
        # payload = 指令码(1字节) + 参数(N字节)
        payload = struct.pack(">B", instruction) + params
        return self._build_generic_packet(self.PID_CMD, payload)

    def build_data_packet(self, data: bytes, is_end: bool = False) -> bytes:
        """
        组装上传/下载用的数据包 (PID=0x02 或 0x08)。
        用于透传指纹图像或特征模板数据。
        
        :param data: 传输的纯数据体 (通常最大 128 字节)
        :param is_end: 是否为最后一包数据（决定 PID 是 0x02 还是 0x08）
        """
        pid = self.PID_END if is_end else self.PID_DATA
        return self._build_generic_packet(pid, data)

    def parse_packet(self, raw_bytes: bytes) -> dict:
        """
        通用解析包方法，支持解析应答包 (0x07) 以及 数据包 (0x02, 0x08)。
        核心优势：使用 length 字段动态截取包体，完美解决串口通信中的“粘包”或“半包”问题。
        """
        # 安全校验：最短的数据包(空Payload)至少为 11 字节
        # 结构：包头(2) + 地址(4) + PID(1) + 长度(2) + 校验和(2) = 11
        if not raw_bytes or len(raw_bytes) < 11:
            return {"status": "error", "msg": "数据包长度不足或为空"}

        # 1. 提取固定头部信息 (前 9 个字节)
        # 格式 >H I B H 对应: 头(2), 地址(4), PID(1), 长度(2)
        head, _, pid, length = struct.unpack(">H I B H", raw_bytes[:9])

        # 包头合法性校验
        if head != self.head:
            return {"status": "error", "msg": f"包头错误: {hex(head)}"}

        # 2. 计算当前这一个包的理论总长度
        # 期望总长度 = 固定前缀9字节 (头2+地址4+PID1+长度2) + length字段指明的后缀长度
        expected_total_len = 9 + length
        
        # 检查缓冲区数据是否接收完整
        if len(raw_bytes) < expected_total_len:
            return {"status": "error", "msg": f"数据不完整，期望 {expected_total_len} 字节，实际 {len(raw_bytes)} 字节"}

        # 3. 精确提取并校验 Checksum
        # 提取最后2个字节作为接收到的校验和。
        # 注意：这里不用 [-2:]，是为了防止 raw_bytes 尾部粘连了下一个包的数据。
        recv_checksum = struct.unpack(">H", raw_bytes[expected_total_len-2 : expected_total_len])[0]
        
        # 本地计算校验和：手册规定从 PID (索引6) 开始算，一直算到校验和之前
        calc_checksum = sum(raw_bytes[6 : expected_total_len-2]) & 0xFFFF

        if recv_checksum != calc_checksum:
            return {"status": "error", "msg": f"校验和错误! 期望:{hex(calc_checksum)} 实际:{hex(recv_checksum)}"}

        # 4. 提取有效载荷 Payload (剥离了头9字节和尾2字节)
        payload = raw_bytes[9 : expected_total_len-2]

        # 5. 构造基础解析结果
        result = {
            "status": "ok",
            "pid": pid,
            "length": length,
            # 方便外部调用者精准切掉已经处理完的数据，留下 remain_bytes 继续下一次解析
            "raw_packet": raw_bytes[:expected_total_len],  
            "remain_bytes": raw_bytes[expected_total_len:] 
        }

        # 6. 根据包类型（PID）进行针对性业务解析
        if pid == self.PID_ACK:
            # 应答包: payload 第一个字节必定是确认码 (0x00 代表成功)
            if len(payload) >= 1:
                result["confirm_code"] = payload[0]
                result["params"] = payload[1:] # 紧随其后的可能是返回数据(如有效模板个数)
            else:
                return {"status": "error", "msg": "应答包缺少确认码"}
        elif pid in (self.PID_DATA, self.PID_END):
            # 数据包: payload 里面全都是纯净的图像或特征数据
            result["data"] = payload
        else:
            # 其他预料之外的包类型
            result["payload"] = payload 

        return result

    def parse_ack(self, raw_bytes: bytes) -> dict:
        """
        专属入口：专门用于验证并解析“应答包 (PID=0x07)”。
        常用于普通指令发送后的同步结果校验。
        """
        res = self.parse_packet(raw_bytes)
        
        # 进一步校验：如果包是完好的，但它不是应答包，则拦截报错
        if res.get("status") == "ok" and res.get("pid") != self.PID_ACK:
            return {"status": "error", "msg": f"期望收到应答包，实际收到 PID: {hex(res.get('pid', 0))}"}
            
        return res