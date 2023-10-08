before_trans = input("输入转换前温度(摄氏度末尾加c，华氏度末尾加f):")
if before_trans[-1] == 'c':
    after_trans = eval(before_trans[0:-1]) * 1.8 + 32
    print("{} 摄氏度(℃) = {:.2f} 华氏度(℉)".format(before_trans[0:-1], after_trans))
elif before_trans[-1] == 'f':
    after_trans = (eval(before_trans[0:-1]) - 32) / 1.8
    print("{} 华氏度(℉) = {:.2f} 摄氏度(℃)".format(before_trans[0:-1], after_trans))
else:
    print("格式错误，重新输入")