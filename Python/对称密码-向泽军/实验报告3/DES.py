import re
import random

# 字符串转为Unicode编码
def string_to_unicode_binary(input_string):
    unicode_binary = ""
    for char in input_string:
        unicode_value = ord(char)
        # 转换为二进制并填充到16位
        unicode_binary += bin(unicode_value)[2:].zfill(16)
    return unicode_binary

# Unicode编码转为字符串
def unicode_binary_to_string(unicode_binary):
    result = ""
    for i in range(0, len(unicode_binary), 16):
        unicode_value = int(unicode_binary[i : i + 16], 2)
        result += chr(unicode_value)
    return result

# DES函数
def DES(text, mode):
    # 将文本分段
    text = text_slice(text)
    for i in range(len(text)):
        text[i] = initial_permutation(text, mode)
        for _ in range(16):
            text[i] = DES_round(text[i], mode)
    text_processed = text
    return text_processed

# 文本切片函数
def text_slice(text):
    text_slice = [text[i : i + 64] for i in range(0, len(text), 64)]
    return text_slice

# IP置换函数
def initial_permutation(text, mode):
    # 初始置换表
    ip_table = [
        58, 50, 42, 34, 26, 18, 10, 2,
        60, 52, 44, 36, 28, 20, 12, 4,
        62, 54, 46, 38, 30, 22, 14, 6,
        64, 56, 48, 40, 32, 24, 16, 8,
        57, 49, 41, 33, 25, 17, 9, 1,
        59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5,
        63, 55, 47, 39, 31, 23, 15, 7
    ]
    if mode == 'e':
        # 进行初始置换操作
        text_ip = [text[ip - 1] for ip in ip_table]
    else:
        # 进行逆置换操作
        for i, ip in enumerate(ip_table):
            text_ip[ip - 1] = text[i]
    # 将列表转换回字符串
    text_ip = ''.join(text_ip)
    return text_ip

# 轮函数
def DES_round(text, mode):
    left = text[0:32]
    return text


# 输入待处理文本
# text = input("请输入文本:")
text = "Aefwgrfbdvewgrhdtfngbdvsegrfbcxdvsegrfbcxdzscafedgrfbcxdzsefrgcvbxdzsefrvcxdzsefrgvbcxdfvgrcxdfgbvcxdfgbvcxdfbcgvd"

text_unicode_binary = string_to_unicode_binary(text)
# 计算需要添加的填充字符个数
padding = len(text_unicode_binary) % 64
# 若长度不符则随机填充末尾
if padding > 0:
    text_unicode_binary += "".join(
        str(random.randint(0, 1)) for _ in range(64 - padding)
    )
# DES处理
text_processed = DES(text_unicode_binary, 'e')
print(text_processed)
