num = eval(input("输入一个整数或浮点数:"))
if isinstance(num, int):
    print("{:-^20}".format(pow(num, 3)))
else:
    print("{:-^20.2f}".format(pow(num, 3)))
