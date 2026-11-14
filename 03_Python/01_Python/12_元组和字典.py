# 不确定需要多少个参数，这时候，可以用可变参数，也叫不定长参数
# 形参的前面加1个*，这个形参就是 元组型可变参数   arguments  ===》 args
# 只需要定义时，加*，使用时，不需加

# 形参名字加1个**，字典型可变参数，使用时，无需加*
# key word arguments  ===》 kwargs


def func(temp, *args, **kwargs):
    print(f"temp:{temp}")
    print(f"args:{args}")
    print(f"kwagrs:{kwargs}")

func(1, 2, 3, 4, 5, a = 1, b=2, c= 3)
