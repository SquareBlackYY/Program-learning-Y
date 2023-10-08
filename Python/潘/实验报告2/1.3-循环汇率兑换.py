while True:
    before_exchange = input("输入兑换前数额(人民币末尾加y，美元末尾加d):")
    if before_exchange[-1] == "y":
        after_exchange = eval(before_exchange[0:-1]) / 6
        print(
            "{} 元(CNY) = {:.2f} 美元(USD)".format(before_exchange[0:-1], after_exchange)
        )
    elif before_exchange[-1] == "d":
        after_exchange = eval(before_exchange[0:-1]) * 6
        print(
            "{} 美元(USD) = {:.2f} 元(CNY)".format(before_exchange[0:-1], after_exchange)
        )
    else:
        print("格式错误，重新输入")
