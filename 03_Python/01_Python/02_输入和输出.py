"""
接收内容的变量 = input("提示信息")
1. 输入内容，不按回车，阻塞
2. 不管输入任意内容，变量类型都是字符串
"""


# 1. 需求:
# ● 用户输入整型变量a
# ● 用户输入整形变量b
# ● 计算输出a+b=?
a = int(input("请输入整形a："))
b = int(input("请输入整形b："))
print("a + b =",a + b)

# 2. 超市买苹果计算金额
# 需求:	
# ●  收银员输入苹果的价格price，单位：元/斤
# ●  收银员输入用户购买苹果的重量weight，单位：斤
# ●   计算并输出付款金额:xxx元
price = float(input("请输入苹果价格(元/斤)："))
weight = float(input("请输入苹果重量(斤)："))
print(f"付款金额：{price * weight}")

# 3. 需求:
# ●  在控制台依次提示用户输入：姓名name、公司com、职位title、电话telephone、邮箱email
# ●  按照以下格式输出：
name = input("姓名：")
com = input("公司：")
title = input("职位：")
telephone = input("电话：")
email = input("邮箱：")
print(f"公司名称：{com}")
print() # 默认打印换行 
print(f"姓名(职位)：{name}({title})")
print() # 默认打印换行 
print(f"电话：{telephone}")
print(f"邮箱：{email}")

# 
print("%s" % com)
print("%s (%s)" % (name, title))  # 两个参数需要括号括起来