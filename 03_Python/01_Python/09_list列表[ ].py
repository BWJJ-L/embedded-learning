# list 列表的基础语法
data = ["lyf", "zwp", "fc", "zl", "lwh", "jdr", 2026]
print(data, type(data))
data = []
print(data, type(data))

data = ["lyf", "zwp", "fc", "zl", "lwh", "jdr", 2026]
for i in range(0, 7):           # list正的索引从0开始
    print(f"{i}:{data[i]}", end = " ")
    
print()

for i in range(-1, -8, -1):     # list有负的索引
    print(f"{i}:{data[i]}")





# list列表的常用方法
name_list = ["lyf", "zwp", "fc", "zl", "lwh", "jdr", 2026]
print("====================================================================增")
name_list.append("make")
print(name_list)

num_list = [1, 3, 5]
num_list.append([2, 4, 6])  # [1, 3, 5, [2, 4, 6]]
print(num_list)

num_list = [1, 3, 5]
num_list.extend(["lyf", "2025"])  # [1, 3, 5, 2, 4, 6]
print(num_list)

print("====================================================================删")
name_list = ["lyf", "zwp", "fc", "zl", "lwh", "jdr", 2026]
name_list.pop(0)    # ['zwp', 'fc', 'zl', 'lwh', 'jdr', 2026]
print(name_list)
name_list.pop(-1)   # ['zwp', 'fc', 'zl', 'lwh', 'jdr']
print(name_list)

name_list.remove("lwh") # 移除没有的元素会报错
print(name_list)

print("====================================================================改")
name_list = ["lyf", "zwp", "fc", "zl", "lwh", "jdr", 2026]
for i in range(0, 7):
    name_list[i] = "已修改"
print(name_list)

print("====================================================================查")
name_list = ["lyf", "zwp", "lyf", "zl", "lyf", "jdr", 2026]
length = len(name_list)
print(length)

length = name_list.count("lyf") # 查找指定元素出现了几次
print(length)

pos = name_list.index("jdr")    # 寻找指定元素的索引，索引从0开始
print(pos)

print("====================================================================遍历")
for name in name_list:
    print(name)

print("====================================================================排序和反转")
import random

nums = []
for i in range(0, 10):
    n = random.randint(0, 100)
    nums.append(n)
print(nums)

nums.sort() # 默认正向排序
print(nums)

nums.sort(reverse = True)   # 反转序列开启
print(nums)

# 想让列表变成不可修改的，可以转换成元组类型