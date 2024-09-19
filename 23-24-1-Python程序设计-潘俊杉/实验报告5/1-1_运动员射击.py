import random

score = []
for i in range(3):
    x = random.randint(-10, 10)
    y = random.randint(-10, 10)
    if x ** 2 + y ** 2 > 100:
        score.append(0)
    else:
        score.append(10 - int((x ** 2 + y ** 2) ** 0.5))

print("3次射击的环数分别为：{}，最终成绩为：{}环".format(score, max(score)))
