print("人民币与美元双向兑换程序")
print("以c结尾为人民币兑美元，以u结尾为美元兑人民币")
money = input("请输入金额：")
if money[0:-1].isdigit() and money[-1] == 'c':
    usd = eval(money[0:-1]) / 6
    print("{} CNY = {:.2f} USD".format(money[0:-1], usd))
elif money[0:-1].isdigit() and money[-1] == 'u':
    cny = eval(money[0:-1]) * 6
    print("{} USD = {:.2f} CNY".format(money[0:-1], cny))
else:
    print("输入错误")