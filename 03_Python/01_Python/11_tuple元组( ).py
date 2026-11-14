data = ("元组", 123, "lyf", 123.123)
print(data, type(data))

print(data[0])  #
print(data[-1])

# 元组只有一个元素也得加逗号“，”  不然类型不会是<class 'tuple'>类型，会是这单个元素类型
data = ("类", )
print(data, type(data))


print("=================================================数据交换")
# 组包
data = 123, "元组", "没有括号"
print(data, type(data))
# 自动解包
a, b, c = data
print(a, b, c)
print(type(a), type(b), type(c))

# 交换变量
a = 1
b = 2
print(a, b)
a, b = b, a # 直接可以进行交换变量
print(a, b)

print("=================================================内容不可修改")
# 元组内容不可修改
# data[0] = 456
# print(data)  # TypeError: 'tuple' object does not support item assignment

#想要修改需要转换为列表类型
data = list(data)
data[0] = 456
print(data)

