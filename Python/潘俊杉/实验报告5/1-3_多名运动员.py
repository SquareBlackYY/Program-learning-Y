import random

def shoot(score):
    for i in range(3):
        x = random.randint(-10, 10)
        y = random.randint(-10, 10)
        if x ** 2 + y ** 2 > 100:
            score.append(0)
        else:
            score.append(10 - int((x ** 2 + y ** 2) ** 0.5))

athletes = {}
for i in range(1, 13):
    athletes["运动员" + str(i)] = 0

for i in range(1, 11):
    for j in range(1, 13):
        score = []
        shoot(score)
        athletes["运动员" + str(j)] += max(score)
    athletes = dict(sorted(athletes.items(), key=lambda x: x[1], reverse=True))
    print("第{}轮比赛：\n名次\t运动员\t总成绩".format(i))
    athletes_keys = iter(athletes.keys())
    athletes_values = iter(athletes.values())
    for k in range(1, 13):
        print("{}\t{}\t{}".format(k, next(athletes_keys)[3:], next(athletes_values)))