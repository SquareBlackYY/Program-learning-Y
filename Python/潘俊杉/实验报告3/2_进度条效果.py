import time

length = 50
print("执行开始".center(length // 2, "-"))
t = time.perf_counter()
for i in range(length + 1):
    a = "*" * i
    b = "." * (length - i)
    c = (i / length) * 100
    print("{:3.0f}%[{}->{}]{:.2f}s".format(c, a, b, time.perf_counter() - t), end="\r")
    time.sleep(0.05)
print("\n", "执行结束".center(length // 2, "-"))
