import random

def shoot(score):
    for i in range(3):
        x = random.randint(-10, 10)
        y = random.randint(-10, 10)
        if x ** 2 + y ** 2 > 100:
            score.append(0)
        else:
            score.append(10 - int((x ** 2 + y ** 2) ** 0.5))

total_score = []
for i in range(10):
    score = []
    shoot(score)
    total_score.append(max(score))
print("每轮成绩为：{}，最终成绩为：{}".format(total_score, sum(total_score)))
