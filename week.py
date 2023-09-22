s = ("一二三四五六日")
while True:
    num = input("输入数字:")
    if num.isdigit() and isinstance(eval(num), int) and 1 <= eval(num) <= 7:
        print("星期"+s[eval(num - 1)])
    else:
        print("输入错误")