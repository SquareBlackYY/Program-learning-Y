import random

below_curve = 0
times = 1000000
for _ in range(times):
    x = random.uniform(0, 1)
    y = random.uniform(0, 1)
    below_curve += 1 if y <= x**2 else 0
print("次数:{}, 积分结果{}".format(times, below_curve / times))