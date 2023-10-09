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

# 种子密钥校验
def validate_and_pad_seed_key(seed_key):
    # 检查种子密钥长度是否为56位
    if len(seed_key) != 56:
        raise ValueError("错误！种子密钥长度应为56位。")
    # 检查种子密钥是否只包含0和1
    if not all(bit in ['0', '1'] for bit in seed_key):
        raise ValueError("错误！种子密钥只能包含0和1。")
    # 计算种子密钥中1的个数
    count_ones = seed_key.count('1')
    # 检查种子密钥中1的个数是否为奇数
    if count_ones % 2 != 1:
        raise ValueError("错误！种子密钥中1的个数只能为奇数。")
    # 补充校验位
    parity_bit = '1' if count_ones % 2 == 0 else '0'
    padded_seed_key = seed_key + parity_bit
    # 补充成64位种子密钥
    padded_seed_key += '0' * (64 - len(padded_seed_key))
    return padded_seed_key

# 56位种子密钥校验并补全校验位
padded_seed_key = validate_and_pad_seed_key(seed_key)

# 计算需要添加的填充字符个数并随机填充末尾
padding = len(binary_text) % 64
if padding > 0:
    binary_text += "".join(str(random.randint(0, 1)) for _ in range(64 - padding))