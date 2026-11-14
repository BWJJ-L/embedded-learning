
import random

house_list = [
    [],[],[],
]

names = ["lyf", "fc", "zwp", "lwh", "jdr", "zl", "xhj"]

for name in names:
    list_id = random.randint(0, 2)
    print(id(house_list[list_id]))
    aa = house_list[list_id]    # python赋值是地址赋值
    print(id(aa))
    aa.append(name)
    
print(house_list)