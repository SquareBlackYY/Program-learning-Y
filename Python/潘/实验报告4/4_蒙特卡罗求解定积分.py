import random

times = 10000
result = 0
for i in range(times):
    x = random.uniform(0,1)
    y = random.uniform(0,1)
    f = pow(x, 0.5)
    if y <= f:
        result += 1

print("结果为:{:.4f},".format(result/times))
