
ages = {
    "jack" : 18,
    "mark" : 19,
    "rose" : 20, 
}

# 直接遍历
for key in ages:
    print(key, ages[key])

print(f"============================== {type(ages.keys())}") # <class 'dict_keys'>
# 获取所有key遍历
for key in ages.keys():
    print(key)

print(f"============================== {type(ages.values())}") # <class 'dict_values'>
# 获取所有value遍历
for value in ages.values():
    print(value)

print(f"============================== {type(ages.items())}") # <class 'dict_items'>
for key in ages.items():
    print(key, type(key)) # <class 'tuple'>

# 既然是元组类型，那么就可以进行拆包
for key, value in ages.items():
    print(key, value)


print("创建空的集合和字典时，默认{}是一个字典")
data = {}
print(type(data))

dd = dict({})
ss = set({})
print(dd, type(dd))
print(ss, type(ss))