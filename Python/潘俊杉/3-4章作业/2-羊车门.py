import random

times = 1000000
unchanged_win = 0
changed_win = 0
for _ in range(times):
    door = [0, 0, 0]
    door[random.randint(0, 2)] = 1
    choice = random.randint(0, 2)
    open_door = random.choice([index for index, value in enumerate(door) if value == 0 and index != choice])
    if door[choice]:
        unchanged_win += 1
    elif door[3 - choice - open_door]:
        changed_win += 1
print("经过{}次预测:".format(times))
print("改变选择获胜的概率为:{:}".format(changed_win / times))
print("坚持选择获胜的概率为:{:}".format(unchanged_win / times))