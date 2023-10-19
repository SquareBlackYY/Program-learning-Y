n = int(input("请输入n:"))

for i in range(n + 1):
    print("第{}行：".format(i), end = '')
    for j in range(i + 1):
        print("{}{}{}={}".format(n - j, '-' if n - i - j >= 0 else '+', abs(n - i - j), eval(str(n - j)+('-'if n - i - j >= 0 else '+')+str(abs(n - i - j)))), end = ' ')
    print('')
    