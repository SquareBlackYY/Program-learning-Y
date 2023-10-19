def sort_num(num):
    sorted_num = int(''.join(sorted(list(str(num)))))
    return sorted_num

input_num = int(input("请输入一个整数："))
print("重新排列后的数字：", sort_num(input_num))
