import time

# DES函数
def DES(plain_text, padded_seed_key, mode):
    # 将明文切片
    plain_text = [plain_text[i : i + 64] for i in range(0, len(plain_text), 64)]
    cipher_text = ''
    # 密钥生成
    key_schedule = generate_key_schedule(padded_seed_key)
    for i in range(len(plain_text)):
        # 进行初始IP置换
        plain_text[i] = initial_permutation(plain_text[i], 1)
        # 进行16轮轮函数
        for j in range(16):
            plain_text[i] = DES_round(plain_text[i], key_schedule[j if mode == 'e' else 15 - j])
        # 进行左右32bit交换
        plain_text[i] = plain_text[i][32:] + plain_text[i][:32]
        # 进行逆置换
        plain_text[i] = initial_permutation(plain_text[i], 0)
        cipher_text += plain_text[i]
    return cipher_text

# IP置换函数
def initial_permutation(bits, mode):
    # 初始置换表
    ip_table = [
        58, 50, 42, 34, 26, 18, 10, 2,
        60, 52, 44, 36, 28, 20, 12, 4,
        62, 54, 46, 38, 30, 22, 14, 6,
        64, 56, 48, 40, 32, 24, 16, 8,
        57, 49, 41, 33, 25, 17,  9, 1,
        59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5,
        63, 55, 47, 39, 31, 23, 15, 7
    ]
    # 创建长度64的空列表
    bits_ip = [''] * 64
    if mode == 1:
        # 进行初始置换操作
        bits_ip = [bits[ip - 1] for ip in ip_table]
    else:
        # 进行逆置换操作
        for i, ip in enumerate(ip_table):
            bits_ip[ip - 1] = bits[i]
    # 将列表转换回字符串
    bits_ip = ''.join(bits_ip)
    return bits_ip

# 轮函数
def DES_round(plain_text, key_slice):
    # 切片为左右两半
    left = plain_text[:32]
    right = plain_text[32:]
    # 扩展右半
    expanded_right = DES_expand(right)
    # 右半与密钥异或
    xored_bits = xor(expanded_right, key_slice)
    # 异或后进行压缩运算(S盒)
    compressed_bits = DES_compress(xored_bits)
    # 压缩后进行P置换
    permuted_bits = DES_permute(compressed_bits)
    # 左半与P置换后异或
    new_right = xor(left, permuted_bits)
    # 左右交换
    result = right + new_right
    return result

# 选择扩展运算函数
def DES_expand(bits):
    # 扩展表
    expansion_table = [
        32,  1,  2,  3,  4,  5,
         4,  5,  6,  7,  8,  9,
         8,  9, 10, 11, 12, 13,
        12, 13, 14, 15, 16, 17,
        16, 17, 18, 19, 20, 21,
        20, 21, 22, 23, 24, 25,
        24, 25, 26, 27, 28, 29,
        28, 29, 30, 31, 32,  1
    ]
    expanded_bits = ""
    for index in expansion_table:
        expanded_bits += bits[index - 1]
    return expanded_bits

# 异或函数
def xor(a, b):
    result = ""
    for i in range(len(a)):
        if a[i] == b[i]:
            result += '0'
        else:
            result += '1'
    return result

# S盒压缩
def DES_compress(bits):
    # S盒
    s_box = [
        # S1
        [
            [14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7],
            [0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8],
            [4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0],
            [15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13]
        ],
        # S2
        [
            [15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10],
            [3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5],
            [0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15],
            [13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9]
        ],
        # S3
        [
            [10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8],
            [13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1],
            [13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7],
            [1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12]
        ],
        # S4
        [
            [7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15],
            [13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9],
            [10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4],
            [3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14]
        ],
        # S5
        [
            [2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9],
            [14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6],
            [4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14],
            [11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3]
        ],
        # S6
        [
            [12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11],
            [10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8],
            [9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6],
            [4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13]
        ],
        # S7
        [
            [4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1],
            [13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6],
            [1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2],
            [6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12]
        ],
        # S8
        [
            [13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7],
            [1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2],
            [7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8],
            [2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11]
        ]
    ]
    compressed_bits = ""
    for i in range(0, 48, 6):
        row = int(bits[i] + bits[i + 5], 2)
        col = int(bits[i + 1 : i + 5], 2)
        value = s_box[i // 6][row][col]
        compressed_bits += format(value, '04b')
    return compressed_bits

# 置换运算P
def DES_permute(bits):
    # 置换表
    permutation_table = [
        16,  7, 20, 21,
        29, 12, 28, 17,
         1, 15, 23, 26,
         5, 18, 31, 10,
         2,  8, 24, 14,
        32, 27,  3,  9,
        19, 13, 30,  6,
        22, 11,  4, 25
    ]
    permuted_bits = ""
    for index in permutation_table:
        permuted_bits += bits[index - 1]
    return permuted_bits

# 种子密钥密钥扩展
def generate_key_schedule(seed_key):
    # PC-1表，用于密钥置换
    pc1_table = [
        57, 49, 41, 33, 25, 17,  9,
         1, 58, 50, 42, 34, 26, 18,
        10,  2, 59, 51, 43, 35, 27,
        19, 11,  3, 60, 52, 44, 36,
        63, 55, 47, 39, 31, 23, 15,
         7, 62, 54, 46, 38, 30, 22,
        14,  6, 61, 53, 45, 37, 29,
        21, 13,  5, 28, 20, 12,  4
    ]
    # PC-2表，用于生成子密钥
    pc2_table = [
        14, 17, 11, 24,  1,  5,
         3, 28, 15,  6, 21, 10,
        23, 19, 12,  4, 26,  8,
        16,  7, 27, 20, 13,  2,
        41, 52, 31, 37, 47, 55,
        30, 40, 51, 45, 33, 48,
        44, 49, 39, 56, 34, 53,
        46, 42, 50, 36, 29, 32
    ]
    # 对种子密钥进行PC-1置换
    permuted_key = [seed_key[pc1 - 1] for pc1 in pc1_table]
    # 分割置换后的密钥为左半部分和右半部分
    left = permuted_key[:28]
    right = permuted_key[28:]
    key_schedule = []
    # 生成16个子密钥
    for round_num in range(16):
        # 根据轮数计算左移的位数
        if round_num in [0, 1, 8, 15]:
            num_shifts = 1
        else:
            num_shifts = 2
        # 左移左半部分和右半部分
        left = left[num_shifts:] + left[:num_shifts]
        right = right[num_shifts:] + right[:num_shifts]
        # 合并左半部分和右半部分
        combined_key = left + right
        # 根据PC-2表进行置换，生成子密钥
        sub_key = [combined_key[pc2 - 1] for pc2 in pc2_table]
        # 将子密钥添加到密钥流中
        key_schedule.append(sub_key)
    return key_schedule

# 二进制转十六进制
def binary_to_hex(binary_string):
    hex_string = hex(int(binary_string, 2))[2:].zfill(len(binary_string) // 4)
    return hex_string

# 十六进制转二进制
def hex_to_binary(hex_string):
    binary_string = bin(int(hex_string, 16))[2:].zfill(len(hex_string) * 4)
    return binary_string

# 输入种子密钥
seed_key = "1f1f1f1f0e0e0e0e"
padded_seed_key = hex_to_binary(seed_key)

# 输入待处理文本
# text = "0123456789abcdef"
# binary_text = hex_to_binary(text)

# 从文件中读取待处理文本
with open('input.txt', 'r') as file:
    text = file.read().strip()
binary_text = hex_to_binary(text)

# 开始计时
start_time = time.time()

# DES加密处理
binary_encrypted = DES(binary_text, padded_seed_key, 'e')

# 计时结束
end_time = time.time()

hex_encrypted = binary_to_hex(binary_encrypted)
# print("加密结果:", hex_encrypted)

# 写入加密结果到output.txt
with open('output.txt', 'w') as file:
    file.write(hex_encrypted)

execution_time = end_time - start_time
print("程序用时: %.2f s" % execution_time)

# DES解密处理
# binary_decrypted = DES(binary_encrypted, padded_seed_key, 'd')
# hex_decrypted = binary_to_hex(binary_decrypted)
# print("解密结果:", hex_decrypted)

# 比特按位取反
# def bits_reverse(bits):
#    bits_reversed = ''.join('0' if i == '1' else '1' for i in bits)
#    return bits_reversed

# binary_decrypted = DES(bits_reverse(binary_encrypted), bits_reverse(padded_seed_key), 'd')
# hex_decrypted = binary_to_hex(binary_decrypted)
# print("比特翻转解密结果:", hex_decrypted)