"""
打印出所有"水仙花数"，所谓"水仙花数"是指一个三位数[100, 1000)，其各位数字立方和等于该数本身。
例如：153是一个"水仙花数"，因为153=1的三次方＋5的三次方＋3的三次方。
"""

# num不能再次被赋值，num此时已经是100 到 999的值 
for num in range(100, 1000):
    if num == ((num//100) ** 3 + (num%100//10) ** 3 + (num%10) ** 3):
        print(num)



"""
判断登录密码hhew2383dW_fkf&E@^是否合法。
1. 密码必须是数字、字母(大小写都可以)、和下划线，否则不合法
ok_content = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_"
2. 如果密码合法,就输出"密码合法"
"""
passward = "hhew2383dW_fkfE"
ok_content = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_"
for temp in passward:
    if temp not in ok_content:
        print("密码不合法！")
        break
else:
    print("密码合法！")
    


