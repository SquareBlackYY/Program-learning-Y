import time

start_time = time.time()
sum_1 = sum(range(1, 100000001))
end_time = time.time()
time_1 = end_time - start_time
print("方法一的累加结果为: {}".format(sum_1))
print("方法一的运行时间为: {:.4f}秒".format(time_1))

start_time = time.time()
sum_2 = (1 + 100000000) * 100000000 // 2
end_time = time.time()
time_2 = end_time - start_time
print("方法二的累加结果为: {}".format(sum_2))
print("方法二的运行时间为: {:.4f}秒".format(time_2))
