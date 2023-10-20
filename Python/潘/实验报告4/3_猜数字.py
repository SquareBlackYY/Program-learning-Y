import random

while True:
    target = random.randint(0, 100)
    while True:
        try:
            guess = int(input("请输入一个猜测的数字(0-100):"))
            if guess < 0 or guess > 100:
                print("请输入0到100之间的数字!")
                continue
            if guess < target:
                print("猜小了!")
            elif guess > target:
                print("猜大了!")
            else:
                print("恭喜你,猜对了!")
                break
        except ValueError:
            print("请输入一个有效的整数!")

    if input("是否再玩一轮？(Y/N):").upper() != "Y":
        break
    