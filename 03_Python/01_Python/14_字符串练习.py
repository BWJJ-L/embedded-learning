"""
● 用户名和密码格式校验程序
● 要求从键盘输入用户名和密码，分别校验格式是否符合规则
  ○ 如果符合，打印用户名合法，密码合法
  ○ 如果不符合，打印出不符合的原因，并提示重新输入
● 用户名长度6-20，用户名必须以字母开头
● 密码长度至少6位，不能为纯数字，不能有空格
"""

while True:
    account = input("请输入账号：")
    if not (6 <= len(account) <= 20):
        print("用户名长度6-20！请重新输入！")
        continue
    
    if account[0].isalpha():
        print("账户输入正确！")
        break
    else:
        print("用户名必须以字母开头！请重新输入！")

while True:
    password = input("请输入密码：")
    if len(password) < 6:
        print("密码长度至少6位！请重新输入！")
        continue
    
    if password.isdecimal():
        print("不能为纯数字")
        continue
    
    if " " in password:
        print("不能有空格！请重新输入！")
        continue
    
    print("密码输入正确！")
    break
        
    
    
    