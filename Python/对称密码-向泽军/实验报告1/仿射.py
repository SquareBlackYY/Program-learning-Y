import os
import math

# 主体循环
while True:
    os.system("cls")
    print("========实验报告1:仿射密码加解密程序========")

    # 输入并处理乘法密钥
    while True:
        key_a = input("请输入乘法密钥:")
        if (
            key_a.isdigit()
            and isinstance(eval(key_a), int)
            and 0 <= eval(key_a) <= 26
            and math.gcd(eval(key_a), 26) == 1
        ):
            key_a = eval(key_a)
            break
        else:
            print("密钥不满足要求或格式错误")

    # 输入并处理加法密钥
    while True:
        key_b = input("请输入加法密钥:")
        if key_b.isdigit() and isinstance(eval(key_b), int) and 0 <= eval(key_b) <= 26:
            key_b = eval(key_b)
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

    # 选择文本输入模式
    while True:
        mode_input = input("请选择文本输入方式(f:从文件输入,k:从键盘输入):")
        if mode_input in ["f", "k"]:
            break
        else:
            print("输入错误")

    # 输入文本
    while True:
        if mode_input == "f":
            file_path = input("请输入文件路径(文件名):")
            break
        else:
            text = input("输入文本:")
            break

    # 处理部分
    result = ""
    if mode_input == "f":
        with open(file_path, "r") as file:
            text = file.read()

    for char in text:
        if char.isalpha():
            if char.isupper():
                base = ord("A")
            else:
                base = ord("a")

            if mode == "e":
                encrypted_char = chr((key_a * (ord(char) - base) + key_b) % 26 + base)
            else:
                inverse_key_a = 0
                for i in range(26):
                    if (key_a * i) % 26 == 1:
                        inverse_key_a = i
                        break
                decrypted_char = chr(
                    (inverse_key_a * (ord(char) - base - key_b)) % 26 + base
                )

            result += encrypted_char if mode == "e" else decrypted_char
        else:
            result += char

    # 输出结果
    print("处理结果:{}".format(result))
    print("==========================================")
    choice = input("是否继续使用程序?(y/n):")
    if choice != "y":
        break
