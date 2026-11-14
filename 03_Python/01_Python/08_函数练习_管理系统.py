
card_list = [
    ["lyf", "1234126", "2231339", "123@qq.com"],
    ["zwp", "7890111", "3122345", "456@qq.com"],
    ["jdr", "1357120", "4451126", "789@qq.com"],
]

def card_menu():
    print("*" * 50)
    print("欢迎使用【名片管理系统】")
    print("1. 新建名片")
    print("2. 显示全部")
    print("3. 查询名片")
    print("4. 删除名片")
    print("0. 退出系统")
    print("*" * 50)

# =================================== 新建名片
def card_add():
    name = input("请输入姓名：")
    phone = input("请输入电话：")
    QQ = input("请输入QQ：")
    mail = input("请输入邮箱：")
    card = [name, phone, QQ, mail]
    card_list.append(card)
    print("新建成功")

# =================================== 显示全部名片
def card_display():
    print("姓名\t电话\t\tQQ\t\t邮箱")
    for i in range(len(card_list)):
        print(f"{card_list[i][0]}\t{card_list[i][1]}\t\t{card_list[i][2]}\t\t{card_list[i][3]}")
    print("显示成功")

# =================================== 查询名片
def card_search():
    name_search = input("请输入要查找的姓名：")
    for i in range(len(card_list)):
        if card_list[i][0] == name_search:
            print("姓名", "电话", "QQ", "邮箱", sep = "\t")
            print(card_list[i][0], card_list[i][1], card_list[i][2], card_list[i][3], sep = "\t")
            print("查询成功")
            return card_list[i]
    else:
        print("查询失败，无此人！")

# =================================== 删除名片
def card_delete():
    name_search = input("请输入要删除的名片姓名：")
    for card in card_list:
        if name_search == card[0]:
            card_list.remove(card)
            print("删除成功")
    


while True:
    card_menu()
    options = int(input("请输入选项:"))
    if options == 1:
        card_add()
    elif options == 2:
        card_display()
    elif options == 3:
        card_search()
    elif options == 4:
        card_delete()
    elif options == 0:
        print("退出成功！")
        # break
        exit(0)
    else:
        print("请输入正确选项！")

