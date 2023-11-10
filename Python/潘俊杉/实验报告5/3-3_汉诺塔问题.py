def Hanoi(n, A, B, C):
    if n == 1:
        print("{} -> {}".format(A, C))
    else:
        Hanoi(n - 1, A, C, B)
        print("{} -> {}".format(A, C))
        Hanoi(n - 1, B, A, C)

n = eval(input("请输入汉诺塔的层数："))
Hanoi(n, "A", "B", "C")