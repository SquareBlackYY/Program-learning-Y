import random

times = 1000000
i_list = [0, 0, 0, 0, 0, 0]
print("以下为概率表:")
print("{:>7}{:>7}{:>7}{:>7}{:>7}{:>7}".format(1, 2, 3, 4, 5, 6))
for _ in range(times):
    i_list[random.randint(0, 5)] += 1
print("{:>7.4}{:>7.4}{:>7.4}{:>7.4}{:>7.4}{:>7.4}".format(i_list[0] / times, i_list[1] / times, i_list[2] / times, i_list[3] / times, i_list[4] / times, i_list[5] / times))