# 通过集合类型转换，可以给列表元素去重
# 集合打印出来没有固定的顺序，全是随机顺序打印
ss = {"孙悟空", "唐僧", "沙和尚", "孙悟空", 1, 2, 3, 3, 2, 1}
print(ss, type(ss))

for ele in ss:
    print(ele)
    
# ========================================== 集合只能 增加和删除
ss.add("猪八戒")    # 增加元素，打印依旧是随机
print(ss)

ss.remove("孙悟空") # 删除直接把所有的“孙悟空”删除，包括重复的
print(ss)

# discard : 如果元素是集合的成员，则从集合中移除该元素。如果元素不是集合的成员，则不执行任何操作。
ss.discard("白骨精")# 安全移除，若条目不存在，不会报错
print(ss)

print(ss.pop())    # 随机移除元素
print(ss)

ss.clear()  # 清空全部元素
print(ss, type(ss))

# ========================================== 集合还能用来给列表去重
data = [1, 2, 3, 4, 4, 3, 2, 1]
print(data, type(data)) # [1, 2, 3, 4, 4, 3, 2, 1] <class 'list'>
temp = set(data)
print(temp, type(temp)) # {1, 2, 3, 4} <class 'set'>
data = list(temp)
print(data, type(data)) # [1, 2, 3, 4] <class 'list'>

print("===========================简略操作后如下")
data = [1, 2, 3, 4, 4, 3, 2, 1]
data = list(set(data))
print(data, type(data)) # [1, 2, 3, 4] <class 'list'>