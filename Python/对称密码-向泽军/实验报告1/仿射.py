import os
import math

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

# 输入文本并处理
text = input("输入文本:")
result = ""
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
input("程序运行结束")