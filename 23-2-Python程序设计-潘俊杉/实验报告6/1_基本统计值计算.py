def average(data):
    return sum(data) / len(data)

def standard(data):
    aver = average(data)
    return (sum((num - aver) ** 2 for num in data) / len(data)) ** 0.5

def median(data):
    data = sorted(data)
    length = len(data)
    if length % 2 == 0:
        return (data[length // 2 - 1] + data[length // 2]) / 2
    else:
        return data[length // 2]

data = sorted(list(map(int, input("请输入一组数据,空格分隔:").split())))
print("平均值:{:.3f}\n标准差:{:.3f}\n中位数:{:.3f}".format(average(data), standard(data), median(data)))
