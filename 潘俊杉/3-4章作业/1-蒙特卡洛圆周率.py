import random

def pi(times):
    points_in_round = 0
    for i in range(times):
        x = random.uniform(0,1)
        y = random.uniform(0,1)
        if x**2 + y**2 <= 1:
            points_in_round += 1
    return points_in_round / times * 4

print("次数:{:8},".format(100), "π =" , pi(100))
print("次数:{:8},".format(10000), "π =" , pi(10000))
print("次数:{:8},".format(10000000), "π =" , pi(10000000))