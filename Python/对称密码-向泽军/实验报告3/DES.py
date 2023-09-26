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
def DES(text):
    # 将文本分段
    text_slice = [text[i : i + 64] for i in range(0, len(text), 64)]

    text_processed = text_slice
    return text_processed


# def DES_round():
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
text_processed = DES(text_unicode_binary)
print(text_processed)
