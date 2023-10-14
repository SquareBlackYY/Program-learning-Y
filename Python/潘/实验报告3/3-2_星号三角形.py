while True:
    n = int(input("输入三角形行数，需要是奇数:"))
    if n % 2 == 1 and n >= 1:
        break
    else:
        print("输入错误")

for i in range(n):
    print("{}{}".format(" " * (n - i), "*" * ((i + 1) * 2 - 1)))
