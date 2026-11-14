from struct import pack

# --- 1. 准备 Python 变量 ---
# 固定头部信息
head = 0xEF01          # 包头
address = 0xFFFFFFFF   # 设备地址
pid = 0x01             # 命令包标识

# 具体的指令和参数
cmd = 0x3C             # 呼吸灯控制指令码
param_func = 0x02      # 功能码：02代表闪烁灯
param_start = 0x04     # 起始颜色：04代表红灯
param_end = 0x04       # 结束颜色：04代表红灯
param_loop = 0x00      # 循环次数：00代表无限循环

# --- 2. 计算包长度 ---
# 手册规定：包长度 = 指令长度 + 所有参数长度 + 校验和长度(2字节)
# 我们这里有：cmd(1字节) + func(1字节) + start(1字节) + end(1字节) + loop(1字节) + 校验和(2字节)
length = 7

# --- 3. 计算校验和 ---
# 手册规定：校验和 = PID + Length + 指令码 + 所有参数，超出2字节溢出忽略
check_code = (pid + length + cmd + param_func + param_start + param_end + param_loop) & 0xffff

# --- 4. 使用 struct.pack 一次性打包！ ---
# 严格按照字节数填坑：
# 格式：> (大端) H(2) I(4) B(1) H(2) B(1) B(1) B(1) B(1) B(1) H(2)
cmd = pack(">HIBHBBBBBH", head, address, pid, length, cmd, param_func, param_start, param_end, param_loop, check_code)
print("指令：", cmd.hex(" "))