
str = "hello world!"
print(str[1])



print("------------------------------判断")
print("aBcDeF".isalpha()) # 纯字母
print("12abc3".isdecimal())  # 是否纯数字 isdigit
print("一二三123".isnumeric())  # 支持汉字的数字

print("abcdef".startswith("ab"))    # 是否包含开头
print("abcdef".startswith("f"))

print("------------------------------查找")
print("abcdef".find("de"))  # 返回索引
print("abcdef".find("g"))   # 找不到内容，返回-1
print("abcddcba".rfind("a"))  # 从右开始查找 rfind
# 第一个参数为需要换的内容，第二个参数为要换成的内容，第三个参数为要换几个第一个参数的内容
print("abcde.abcfgh".replace("b", "F", 1))  

print("------------------------------切割")
number_list = "123|456|789".split("|")  # 用来切割开，类型会变为列表
print(number_list, type(number_list))

name = "张三赵四王五"
print(name[2:4])    # 从索引2开始到4，但是不包含索引4

print("------------------------------去空白")
print("                abc".strip())

