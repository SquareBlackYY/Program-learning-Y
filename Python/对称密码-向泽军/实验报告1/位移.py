import os

os.system("cls")
print("========实验报告1:移位密码加解密程序========")

# 输入并处理密钥
while True:
    key = input("请输入密钥:")
    if key.isdigit() and isinstance(eval(key), int) and 0 <= eval(key) <= 25:
        key = eval(key)
        break
    else:
        print("密钥不满足要求或格式错误")

# 选择加解密操作模式
while True:
    mode = input("请选择操作(e:加密,d:解密):")
    if mode in ["e", "d"]:
        break
    else:
        print("输入错误")

# 选择输入方式
while True:
    mode_input = input("请选择输入方式(s:终端输入,f:文件输入):")
    if mode_input in ["s", "f"]:
        break
    else:
        print("输入错误")

# 输入文本
if mode_input == 's':
    text = input("输入文本:")
else:
    file_name = input("请输入文件名:")
    with open(file_name, 'r') as file:
        text = file.read()

# 处理部分
result = ""
key = key % 26

for char in text:
    if char.isalpha():
        if char.isupper():
            base = ord('A')
        else:
            base = ord('a')

        if mode == "e":
            encrypted_char = chr((ord(char) - base + key) % 26 + base)
        else:
            encrypted_char = chr((ord(char) - base - key) % 26 + base)

        result += encrypted_char
    else:
        result += char

# 输出结果
print("处理结果:{}".format(result))
print("================程序运行结束================")