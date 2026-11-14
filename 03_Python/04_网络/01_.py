
import socket

tcp = socket.socket(family = socket.AF_INET, type = socket.SOCK_STREAM)

addr = ("127.0.0.1", 8080)
tcp.connect(addr)

tcp.send("111".encode("utf-8"))
print("发送完毕")





tcp.close