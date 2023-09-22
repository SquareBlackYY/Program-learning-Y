import os
import random

while True:
    os.system("cls")
    print("猜数字游戏\n范围是1~20的整数")
    num = random.randint(1, 20)
    while True:
        input_num = input("猜猜这个数：")
        if str.isdigit(input_num) and isinstance(eval(input_num), int):
            input_num = int(input_num)
            if num < input_num:
                print("大了")
            elif num > input_num:
                print("小了")
            else:
                print("恭喜你，猜对喽！")
                break
        else:
            print("格式错误！")

    choice = input("你要继续玩吗?(y/n):")
    if choice != "y":
        break
