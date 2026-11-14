from struct import unpack

# --- 1. 准备原始二进制流 ---
raw_bytes = bytearray([0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x07,0x00,0x03,0x00,0x00,0x0A])
#                       0    1    2    3    4     5    6    7   8    9     10
#                                                                          -2 -1
#                                                     开始          结束         sum(raw_bytes[6:10])    

# --- 2. 使用 struct.unpack 一次性解包！ ---
# 手册规定应答包格式对应：
# > (大端) H(包头2字节) I(地址4字节) B(PID1字节) H(长度2字节) B(确认码1字节) H(校验和2字节)
# 总计：2 + 4 + 1 + 2 + 1 + 2 = 12 字节
# head, address, pid, length, confirm_code, recv_checksum
head, address, pid, length, confirm_code, recv_checksum = unpack(">HIBHBH", raw_bytes)

# --- 3. 提取业务结果 (判断执行成败) ---
# 检查包头和包标识
if head != 0xef01 or address != 0xffffffff:
    print("包头和包标识不通过")
    exit(0)

# --- 4. 验证校验和 (非常重要，防范串口丢包错乱) ---
# 手册规定：校验和是从包标识至校验和之间所有字节之和 
# 这里包含：包标识(pid) + 包长度(我们用拆解的字节算，实际就是 length 的高低位之和) + 确认码(confirm_code)

# 为了严谨，我们直接用截取原始字节流的方式累加，因为长度字段本身占 2 字节
# 数据包从第 6 个字节开始是 PID，一直到倒数第 2 个字节之前
# 切片 raw_bytes[6:-2] 提取到的内容是: 07 00 03 00
if (sum(raw_bytes[6:10]) & 0xffff) != recv_checksum:
    print("校验不通过")
    exit(0)
    
if  confirm_code == 0x00:
    print("响应正常")
else:
    print("响应不正常")