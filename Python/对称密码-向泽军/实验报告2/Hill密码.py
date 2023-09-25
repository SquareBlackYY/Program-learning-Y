import numpy
import os
import math
import random
import re

# 加密函数
def encrypt_hill_cipher(text, key_matrix):
    # 去除非字母字符
    text = re.sub(r'[^A-Z]', '', text)
    print(text)
    # 将明文转换为数字列表
    plain_nums = [ord(c) - ord('A') for c in text]
    # 计算需要添加的填充字符个数并随机填充末尾
    padding = len(plain_nums) % key_matrix.shape[0]
    if padding > 0:
        plain_nums += [random.randint(0, 25) for _ in range(key_matrix.shape[0] - padding)]
    # 将数字列表转换为矩阵
    plain_matrix = numpy.array(plain_nums).reshape(-1, key_matrix.shape[0]).T
    # 使用密钥矩阵对明文矩阵进行加密
    encrypted_matrix = numpy.dot(key_matrix, plain_matrix) % 26
    # 将加密后的矩阵转换为数字列表
    encrypted_nums = encrypted_matrix.T.flatten().astype(int).tolist()
    # 将数字列表转换为密文
    encrypt_text = ''.join(chr(num + ord('A')) for num in encrypted_nums)
    return encrypt_text

# 解密函数
def decrypt_hill_cipher(text, key_matrix):
    # 将密文转换为数字列表
    encrypted_nums = [ord(c) - ord('A') for c in text]
    # 将数字列表转换为矩阵
    encrypted_matrix = numpy.array(encrypted_nums).reshape(-1, key_matrix.shape[0]).T
    # 计算密钥矩阵的逆矩阵
    key_inv = numpy.linalg.inv(key_matrix)
    # 使用逆矩阵对密文矩阵进行解密
    decrypted_matrix = numpy.dot(key_inv, encrypted_matrix) % 26
    # 将解密后的矩阵转换为数字列表
    decrypted_nums = decrypted_matrix.T.flatten().astype(int).tolist()
    # 将数字列表转换为明文
    decrypted_text = ''.join(chr(num + ord('A')) for num in decrypted_nums)
    return decrypted_text

def isint(list):
    for i in list:
        if not i.isdigit() or not isinstance(eval(i), int):
            return False
    return True

# 主程序
os.system("cls")
print("========实验报告2:Hill密码加解密程序========")

# 设置密钥矩阵
while True:
    key_matrix = numpy.array([[0, 0, 0], [0, 0, 0], [0, 0, 0]])
    # key = input("请输入3*3密钥矩阵(空格分隔):").split()
    key = ('6 24 1 13 16 10 20 17 15').split()
    if not len(key) == 9 or not isint(key):
        print("格式错误!")
    else:
        # 将数字赋值给矩阵
        for i in range(3):
            for j in range(3):
                key_matrix[i][j] = int(key[i*3 + j])

        if math.gcd(26, int(numpy.linalg.det(key_matrix))) == 1:
            break
        else:
            print("矩阵不符合要求!")

# 选择加解密操作模式
while True:
    # mode = input("请选择操作(e:加密,d:解密):")
    mode = "e"
    if mode in ["e", "d"]:
        break
    else:
        print("输入错误")

# 输入文本
text = ('There will be people willing to help you ward off all the evil in the world.').upper()
#text = input("请输入文本: ").upper()

# 加密明文
encrypted_text = encrypt_hill_cipher(text, key_matrix)
print("加密后的密文: ", encrypted_text)

# 解密密文
decrypted_text = decrypt_hill_cipher(encrypted_text, key_matrix)
print("解密后的明文: ", decrypted_text)

print("================程序运行结束================")