import random
points_in_round = 0
times = 1
while True:
    x = random.uniform(0,1)
    y = random.uniform(0,1)
    if x**2 + y**2 <= 1:
        points_in_round += 1
    times += 1
    print("次数:{:10},".format(times), "π =" ,points_in_round / times * 4, end = "\r")