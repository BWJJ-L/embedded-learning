def func(num):
    if num < 0:
        print(f"错误：{num}!")
        return
    if num % 2:
        print(f"奇数：{num}!")
    else:
        print(f"偶数：{num}")

for num in range(-10, 10):
    func(num)



def calc(a, b):
    """
    函数返回值可以是多个值
    接收的时候得拿多个变量来接收
    """
    multiple  = a*b
    
    if b == 0:
        print("除数不能为0！")
        return multiple, None   #返回None只是当函数外面解包时，赋值不会报错
    
    Division = a//b
    
    return multiple, Division
# 解包 
a, b = calc(8, 0)
print(f"乘:{a}, 除:{b}")



# g = 123
def func_a():
    a = 456
    # 声明后面用到的g是全局变量g，如果不存在就创建一个全局变量g
    # 如果不加global g的话，这个函数内的g是临时变量g，不是全局的
    global g
    g = 789
    print(a, g)

def func_b():
    print(g)

func_a()
func_b()


# 可以修改换行效果和打印分割效果
a = 10
b = 20
print(f"a:{a}", f"b:{b}", end="+++")
print(f"a:{b}", f"b:{a}", sep="@_*")