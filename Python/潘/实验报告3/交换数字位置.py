def exchange(num, p1, p2):
    num = list(str(num))
    num[p1 - 1], num[p2 - 1] = num[p2 - 1], num[p1 - 1]
    num = "".join(num)
    return int(num)


while True:
    num = input("请输入数字:")
    if num.isdigit() and isinstance(eval(num), int) and eval(num) >= 0:
        num = int(num)
        break
    else:
        print("格式错误")

while True:
    p1 = input("请输入位置1:")
    p2 = input("请输入位置2:")
    if (
        p1.isdigit()
        and p2.isdigit()
        and isinstance(eval(p1), int)
        and isinstance(eval(p2), int)
        and 1 <= eval(p1) <= len(str(num))
        and 1 <= eval(p2) <= len(str(num))
    ):
        p1 = int(p1)
        p2 = int(p2)
        break
    else:
        print("格式错误")

print("交换前:", num)
print("交换后:", exchange(num, p1, p2))
