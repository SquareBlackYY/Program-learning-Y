import random


def shoot(score_ls):
    for i in range(3):
        x = random.randint(-10, 10)
        y = random.randint(-10, 10)
        if x ** 2 + y ** 2 > 100:
            score_ls.append(0)
        else:
            score_ls.append(10 - int((x ** 2 + y ** 2) ** 0.5))


score_ls = []
shoot(score_ls)
print("3次射击的环数分别为：{}，最终成绩为：{}环".format(score_ls, max(score_ls)))