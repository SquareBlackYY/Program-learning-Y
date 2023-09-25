import numpy
import os

# 加密函数
def encrypt_hill_cipher(text, key_matrix):
    # 将明文转换为数字列表
    plain_nums = [ord(c) - ord('A') for c in text]
    # 计算需要添加的填充字符个数
    padding = len(plain_nums) % key_matrix.shape[0]
    if padding > 0:
        plain_nums += [0] * (key_matrix.shape[0] - padding)
    # 将数字列表转换为矩阵
    plain_matrix = numpy.array(plain_nums).reshape(-1, key_matrix.shape[0]).T
    # 使用密钥矩阵对明文矩阵进行加密
    encrypted_matrix = numpy.dot(key_matrix, plain_matrix) % 26
    # 将加密后的矩阵转换为数字列表
    encrypted_nums = encrypted_matrix.T.flatten().astype(int).tolist()
    # 将数字列表转换为密文
    text = ''.join(chr(num + ord('A')) for num in encrypted_nums)
    return text

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
    # 去除填充字符
    decrypted_nums = [num for num in decrypted_nums if num != 0]
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
print("========实验报告1:仿射密码加解密程序========")

# 设置密钥矩阵
key_matrix = numpy.array([[0, 0, 0], [0, 0, 0], [0, 0, 0]])

while True:
    key = input("请输入3*3密钥矩阵(空格分隔):").split()

    if not len(key) == 9 or not isint(key):
        print("格式错误！")
    else:
        # 将数字赋值给矩阵
        for i in range(3):
            for j in range(3):
                key_matrix[i][j] = int(key[i*3 + j])
        break

for row in key_matrix:
        print(row)


# 输入明文
text = input("请输入明文: ").upper()

# 加密明文
text = encrypt_hill_cipher(text, key_matrix)
print("加密后的密文: ", text)

# 解密密文
decrypted_text = decrypt_hill_cipher(text, key_matrix)
print("解密后的明文: ", decrypted_text)

