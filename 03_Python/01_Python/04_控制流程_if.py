

passward = input("请输入密码:")

if passward == "123456":
    print("密码正确！进来！")
else:
    print("密码错误！滚蛋！")



num = int(input("请猜一个数字："))
if num == 5:
    print("牛皮！")
elif num == 4 or num == 6:
    print("差一点就被你猜中了")
elif num <= 3:
    print("小了!")
elif num >= 7:
    print("大了！")
    
    
    
festival = input("今天的节日：")
if festival == "元宵节":
    print("送你一箱脑白金！")
elif festival == "情人节":
    print("送你一朵玫瑰花！")
elif festival == "中秋节":
    print("送你一个五仁月饼！")
else:
    print("每天都是节日哈哈哈！")



# 剪刀 0   石头 1   布 2
import random

user = int(input("请输入0或1或2："))
computer = random.randint(0, 1)

if (user == 0 and computer == 2) or (user == 1 and computer == 0) or (user == 2 and computer == 1):
    print("用户赢！")
elif user == computer:
    print("平局！")
else:
    print("电脑赢！")


print('hello "nihao" wolrd')    # 引号内需要""双引号时打印可以用''单引号