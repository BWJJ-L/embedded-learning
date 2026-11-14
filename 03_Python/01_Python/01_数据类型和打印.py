# 单行注释

"""
多行注释
"""

temp = None
print(temp, type(temp))     # type 查看数据类型

name = "小米"
print(name, type(name))

age = 18
print(age, type(age))

city = "武汉"
print(city, type(city))

height = 1.8
print(height, type(height))

bool1 = 0
print(bool(0), bool(-1), bool(1), bool1, type(bool1))

# 格式化输出，以一定格式输出
print("姓名：", name, "年龄：", age, "位置：", city)
# f字符串格式化   {} 占位符，不输出，替换为变量内容     解释器版本 >= 3.6
print(f"姓名：{name}, 年龄：{age}, 身高：{height}")
# format格式化   {} 占位符，不输出，替换为变量内容
print("我叫 {}, 年龄 {}, 来自 {}".format(name, age, city))


num1 = 1
print(f"num1 = {num1:04d}")     # 保留4位数字，不够的补零
print(f"{num1:02d}")
num2 = 3.1415
print(f"num2 = {num2:.2f}")     # 保留2位小数，四舍五入
print(f"{num2:.3f}")


# 字符串计算
temp = "lei"
name = "abc123"
print(temp + name)
print(name * 5)
