import numpy
import math
import random
import re

# 加密函数
def encrypt_hill_cipher(text, key_matrix):
    # 去除非字母字符
    text = re.sub(r'[^A-Z]', '', text)
    # 将明文转换为数字列表
    plain_nums = [ord(c) - ord('A') for c in text]
    # 计算需要添加的填充字符个数并
    padding = len(plain_nums) % key_matrix.shape[0]
    # 若长度不符则随机填充末尾
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
    # 计算密钥矩阵行列式的值并模26
    key_det = round(numpy.linalg.det(key_matrix) % 26)
    # 求上述值的乘法逆元
    key_det_inv = mod_inverse(key_det, 26)
    # 计算伴随矩阵并转换为整数
    key_adj = numpy.round(numpy.linalg.inv(key_matrix) * numpy.linalg.det(key_matrix)).astype(int)
    # 计算密钥矩阵的逆矩阵
    key_inv = (key_det_inv * key_adj) % 26
    # 使用逆矩阵对密文矩阵进行解密
    decrypted_matrix = numpy.dot(key_inv, encrypted_matrix) % 26
    # 将解密后的矩阵转换为数字列表
    decrypted_nums = decrypted_matrix.T.flatten().astype(int).tolist()
    # 将数字列表转换为明文
    decrypted_text = ''.join(chr(num + ord('A')) for num in decrypted_nums)
    return decrypted_text

# 整数判断函数
def isint(list):
    for i in list:
        if not i.isdigit() or not isinstance(eval(i), int):
            return False
    return True

# 扩展欧几里得算法
def extended_gcd(a, b):
    if b == 0:
        return a, 1, 0
    else:
        gcd, x, y = extended_gcd(b, a % b)
        return gcd, y, x - (a // b) * y

# 求乘法逆元
def mod_inverse(a, m):
    gcd, x, _ = extended_gcd(a, m)
    if gcd == 1:
        return x % m

# 主程序
print("{:=^40}".format('实验报告2:Hill密码加解密程序'))

# 设置密钥矩阵
while True:
    # 创建一个空的密钥矩阵并输入密钥矩阵
    key_matrix = numpy.array([[0, 0, 0], [0, 0, 0], [0, 0, 0]])
    key = input("请输入3*3密钥矩阵(空格分隔):").split()
    # 判断密钥矩阵合法性
    if not len(key) == 9 or not isint(key):
        print("格式错误!")
    else:
        # 满足格式要求后将数字赋值给矩阵
        for i in range(3):
            for j in range(3):
                key_matrix[i][j] = int(key[i*3 + j])
        # 判断是否可逆以及行列式的值是否与26互素
        if math.gcd(26, round(numpy.linalg.det(key_matrix))) == 1:
            break
        else:
            print("密钥矩阵不符合要求!")

# 输入文本
text = input("请输入文本: ").upper()

# 加密明文
encrypted_text = encrypt_hill_cipher(text, key_matrix)
print("加密后的密文: ", encrypted_text)

# 解密密文
decrypted_text = decrypt_hill_cipher(encrypted_text, key_matrix)
print("解密后的明文: ", decrypted_text)

# 程序结束
print("{:=^40}".format('程序运行结束'))