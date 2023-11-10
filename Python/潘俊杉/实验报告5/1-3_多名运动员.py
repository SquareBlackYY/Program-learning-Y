import random


def shoot(score_ls):
    for i in range(3):
        x = random.randint(-10, 10)
        y = random.randint(-10, 10)
        if x ** 2 + y ** 2 > 100:
            score_ls.append(0)
        else:
            score_ls.append(10 - int((x ** 2 + y ** 2) ** 0.5))


athletes = {}
for i in range(1, 13):
    athletes["运动员" + str(i)] = 0

for i in range(1, 11):
    for j in range(1, 13):
        score_ls = []
        shoot(score_ls)
        athletes["运动员" + str(j)] += max(score_ls)
    print("第{}轮比赛排名：{}".format(i, sorted(athletes.items(), key=lambda x: x[1], reverse=True)))