"""
for用于遍历容器（字符串，元组，列表，字典）

for 临时变量 in 容器
    循环体
"""
data = "abc"
for temp in data:
    print(temp)



"""
range(起点，终点，步长)     取不到终点，只能取终点的上一个
步长    下一个数    += 步长
"""
print(list(range(0, 10)))   # [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
print(list(range(0, 10, 2))) # [0, 2, 4, 6, 8]
print(list(range(10)))      # [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
print(list(range(8, -1, -2))) # [8, 6, 4, 2, 0]



for i in range(5):
    print(f"跑步第{i}圈")

# in 也可以单独使用，他是一个运算符，判断元素是否在容器内
print("in" in "whinle")
print("oi" in "while")
print("lyf" not in "qwertyuiop")



"""
循环(while， for)中没有执行到break，循环结束后，自动调用else

else，循环执行完了，并且没有执行break
"""
for i in range(1, 6):
    if i == 3:
        print("结束！")
        break
    print(f"当前值：{i}")
else:
    print("循环执行完毕，没有执行break！")