import random


def shoot(score_ls):
    for i in range(3):
        x = random.randint(-10, 10)
        y = random.randint(-10, 10)
        if x ** 2 + y ** 2 > 100:
            score_ls.append(0)
        else:
            score_ls.append(10 - int((x ** 2 + y ** 2) ** 0.5))


total_score = []
for i in range(10):
    score_ls = []
    shoot(score_ls)
    total_score.append(max(score_ls))
print("每轮成绩为：{}，最终成绩为：{}".format(total_score, sum(total_score)))
