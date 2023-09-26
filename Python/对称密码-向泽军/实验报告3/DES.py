import random

# 字符串转为Unicode编码
def text_to_unicode(text):
    unicode = ""
    for char in text:
        unicode_value = ord(char)
        # 转换为二进制并填充到16位
        unicode += bin(unicode_value)[2:].zfill(16)
    return unicode

# Unicode编码转为字符串
def unicode_to_text(unicode):
    text = ""
    for i in range(0, len(unicode), 16):
        unicode_value = int(unicode[i : i + 16], 2)
        text += chr(unicode_value)
    return text

# DES函数
def DES(unicode, mode):
    # 将Unicode分段
    unicode = unicode_slice(unicode)
    # 初始化处理结果字符串
    unicode_processed = ''
    # 对每一段进行处理
    for i in range(len(unicode)):
        # 进行初始IP置换
        unicode[i] = initial_permutation(unicode[i], mode)
        # 进行16轮轮函数
        for _ in range(16):
            unicode[i] = DES_round(unicode[i], mode)
        # 进行左右32bit交换
        unicode[i] = unicode[i][32:] + unicode[i][:32]
        # 进行逆置换
        unicode[i] = initial_permutation(unicode[i], chr(ord(mode) ^ 1))
        unicode_processed += unicode[i]
    return unicode_processed

# Unicode切片函数
def unicode_slice(unicode):
    unicode_slice = [unicode[i : i + 64] for i in range(0, len(unicode), 64)]
    return unicode_slice

# IP置换函数
def initial_permutation(unicode, mode):
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
    # 创建长度64的空列表
    unicode_ip = [''] * 64
    if mode == 'e':
        # 进行初始置换操作
        unicode_ip = [unicode[ip - 1] for ip in ip_table]
    else:
        # 进行逆置换操作
        for i, ip in enumerate(ip_table):
            unicode_ip[ip - 1] = unicode[i]
    # 将列表转换回字符串
    unicode_ip = ''.join(unicode_ip)
    return unicode_ip

# 轮函数
def DES_round(unicode, mode):
    left = unicode[:32]
    right = unicode[32:]
    #print(left)
    #print(right)
    return unicode


# 输入待处理文本
# text = input("请输入文本:")
text = "Aefwgrfbdvewgrhdtfngbdvsegrfbcxdvsegrfbcxdzscafedgrfbcxdzsefrgcvbxdzsefrvcxdzsefrgvbcxdfvgrcxdfgbvcxdfgbvcxdfbcgvd"

# 将文本转换为Unicode
unicode = text_to_unicode(text)
# 计算需要添加的填充字符个数
padding = len(unicode) % 64
# 若长度不符则随机填充末尾
if padding > 0:
    unicode += "".join(str(random.randint(0, 1)) for _ in range(64 - padding))

# DES加密处理
unicode_encrypted = DES(unicode, 'e')
# 输出加密处理结果
print(unicode_encrypted)

# DES解密处理
unicode_decrypted = DES(unicode_encrypted, 'd')
# 将Unicode转换为文本
text_decrypted = unicode_to_text(unicode_decrypted)
# 输出解密处理结果
print(text_decrypted)