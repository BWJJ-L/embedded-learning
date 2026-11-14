import random

while True:
    game = input("欢迎来到LYF游戏厅,退出输入-1 \n接下来玩什么游戏？ \n“猜拳”还是“猜数字”：")

    if game == "猜数字":
        while True:
            num1 = int(input("请输入一个数字："))
            num2 = random.randint(1, 100)

            while num1 != num2:
                if num1 > num2:
                    print("输入大了！")
                elif num1 < num2:
                    print("太小！")
                    
                num1 = int(input("请输入一个数字："))
                
            print(f"完美-Perfect！结果：{num2}")
            num1 = int(input("结束游玩输入-1, 继续游玩按下1："))
            if num1 == -1:
                break
    elif game == "猜拳":
        while True:
            user = int(input("请输入0或1或2, 退出-1："))
            if user == -1:
                break
            computer = random.randint(0, 1)

            if (user == 0 and computer == 2) or (user == 1 and computer == 0) or (user == 2 and computer == 1):
                print("用户赢！")
            elif user == computer:
                print("平局！")
            else:
                print("电脑赢！")
                
    elif game == "-1":
        break


