"""
定义:      key : value
变量 = {   键1 : 值1,     键2 : 值2,    ……}
键一般是字符串类型，键是唯一的

如果字典里面有重复的键， 后面的元素把前面覆盖了
"""

ages = {
    # 字典内也可以加注释，但是注释得单独一行
    "jack" : 18,
    "mark" : 19,
    "rose" : 20, 
    "rose" : 30,
    # 最后一行逗号可加可不加
}
print(ages, type(ages))

# 字典可以增删改查
print("=====================增加")
ages["tom"] = 21
print(ages)

print("=====================删除")
ages.pop("jack")
print(ages)

print("=====================修改")
ages["mark"] = 100  # 键存在就是修改，键不存在就是增加
print(ages)

print("=====================查找")
print(ages["rose"])
print(ages.get("111"))  # 键不存在返回none

print(f"rose年龄为：{ages['rose']}")