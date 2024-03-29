import time


S_BOX = [
    [0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76],
    [0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0],
    [0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15],
    [0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75],
    [0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84],
    [0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf],
    [0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8],
    [0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2],
    [0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73],
    [0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb],
    [0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79],
    [0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08],
    [0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a],
    [0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e],
    [0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf],
    [0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16]]
'''适用于字节代换的S盒'''

INV_S_BOX = [
    [0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB],
    [0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB],
    [0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E],
    [0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25],
    [0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92],
    [0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84],
    [0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06],
    [0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B],
    [0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73],
    [0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E],
    [0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B],
    [0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4],
    [0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F],
    [0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF],
    [0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61],
    [0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D]]
'''适用于逆字节代换的S盒的逆'''

MIXCOLUMNS_MATRIX = [
    [0x02, 0x03, 0x01, 0x01],
    [0x01, 0x02, 0x03, 0x01],
    [0x01, 0x01, 0x02, 0x03],
    [0x03, 0x01, 0x01, 0x02]]
'''适用于列混淆的矩阵'''

INV_MIXCOLUMNS_MATRIX = [
    [0x0e, 0x0b, 0x0d, 0x09],
    [0x09, 0x0e, 0x0b, 0x0d],
    [0x0d, 0x09, 0x0e, 0x0b],
    [0x0b, 0x0d, 0x09, 0x0e]]
'''适用于逆列混淆的逆矩阵'''

R_CON = [
    0x01, 0x02, 0x04, 0x08, 0x10,
    0x20, 0x40, 0x80, 0x1B, 0x36]
'''轮常量'''


## 文本与矩阵处理函数
def text_to_array(hex_int):
    '''文本转矩阵并分组(按行分组)'''
    hex_str = format(hex_int, "032x")
    return [[[int(hex_str[2 * i:2 * i + 2], 16) for i in range(16)][4 * i + j] for i in range(4)] for j in range(4)]

def key_to_array(hex_int):
    '''密钥转矩阵并分组(按列分组)'''
    hex_str = format(hex_int, "032x")
    return [[[int(hex_str[2 * i:2 * i + 2], 16) for i in range(16)][4 * j + i] for i in range(4)] for j in range(4)]

def array_to_hex(hex_array):
    '''取消分组并将矩阵转换为十六进制字符串'''
    hex_str = ""
    for i in range(16):
        hex_str += format([hex_array[j][i] for i in range(4) for j in range(4)][i], "02x")
    return hex_str


## 轮密钥相关函数

# 轮密钥加
def AddRoundKey(text, key):
    '''轮密钥加'''
    return [[text[i][j] ^ key[i][j] for j in range(4)] for i in range(4)]

# 字节代换
def subBytes(text):
    '''字节代换'''
    global S_BOX
    for i in range(4):
        for j in range(4):
            text[i][j] = S_BOX[text[i][j] // 16][text[i][j] % 16]
    return text

def inv_subBytes(text):
    '''逆字节代换'''
    global INV_S_BOX
    for i in range(4):
        for j in range(4):
            text[i][j] = INV_S_BOX[text[i][j] // 16][text[i][j] % 16]
    return text

# 行移位
def shiftRows(text):
    '''行移位'''
    return [[text[i][(i + j) % 4] for j in range(4)] for i in range(4)]

def inv_shiftRows(text):
    '''逆行移位'''
    return [[text[i][(j - i) % 4] for j in range(4)] for i in range(4)]

# 列混淆
def mixColumns(text):
    '''列混淆'''
    global MIXCOLUMNS_MATRIX
    result = []
    for i in range(4):
        row = []
        for j in range(4):
            mixed_num = 0
            for k in range(4):
                if MIXCOLUMNS_MATRIX[i][k] == 1:
                    mixed_num ^= text[k][j]
                elif MIXCOLUMNS_MATRIX[i][k] == 2:
                    mixed_num ^= ((text[k][j] << 1) ^ ((text[k][j] >> 7) * 0x1B)) % 256
                elif MIXCOLUMNS_MATRIX[i][k] == 3:
                    mixed_num ^= ((text[k][j] << 1) ^ ((text[k][j] >> 7) * 0x1B) ^ text[k][j]) % 256
            row.append(mixed_num)
        result.append(row)
    return result

def inv_mixColumns(text):
    '''逆列混淆'''
    global INV_MIXCOLUMNS_MATRIX
    result = []
    for i in range(4):
        row = []
        for j in range(4):
            inv_mixed_num = 0
            for k in range(4):
                temp = text[k][j]
                for n in range(3):
                    temp = ((temp << 1) ^ ((temp >> 7) * 0x1B)) % 256
                if INV_MIXCOLUMNS_MATRIX[i][k] == 9:
                    inv_mixed_num ^= temp ^ text[k][j]
                elif INV_MIXCOLUMNS_MATRIX[i][k] == 11:
                    inv_mixed_num ^= temp ^ ((text[k][j] << 1) ^ ((text[k][j] >> 7) * 0x1B)) % 256 ^ text[k][j]
                elif INV_MIXCOLUMNS_MATRIX[i][k] == 13:
                    middle = text[k][j]
                    for n in range(2):
                        middle = ((middle << 1) ^ ((middle >> 7) * 0x1B)) % 256
                    inv_mixed_num ^= temp ^ middle ^ text[k][j]
                elif INV_MIXCOLUMNS_MATRIX[i][k] == 14:
                    middle = text[k][j]
                    for n in range(2):
                        middle = ((middle << 1) ^ ((middle >> 7) * 0x1B)) % 256
                    inv_mixed_num ^= temp ^ middle ^ ((text[k][j] << 1) ^ ((text[k][j] >> 7) * 0x1B)) % 256
            row.append(inv_mixed_num)
        result.append(row)
    return result


## 密钥生成相关函数

# 密钥矩阵转置
def key_transpose(key):
    '''密钥矩阵转置'''
    result = []
    for i in range(4):
        col = []
        for j in range(4):
            col.append(key[j][i])
        result.append(col)
    return result

# 密钥字字节代换
def SubWord(key_word):
    '''密钥字字节代换'''
    global S_BOX
    for i in range(4):
        key_word[i] = S_BOX[key_word[i] // 16][key_word[i] % 16]
    return key_word

# 密钥字循环上移一个字节
def RotWord(key_word):
    '''密钥字循环上移一个字节'''
    return [key_word[(i + 1) % 4] for i in range(4)]

# 每轮密钥扩展
def key_extend(key, round_num):
    '''每轮密钥扩展'''
    global R_CON
    key[0] = [SubWord(RotWord(key[3]))[i] ^ key[0][i] ^ R_CON[round_num] for i in range(4)]
    for i in range(1, 4):
        key[i] = [key[i - 1][j] ^ key[i][j] for j in range(4)]
    return key

# 生成密钥表
def generate_key_schedule(key):
    '''生成密钥表'''
    key_schedule = []
    key = key_to_array(key)
    key_schedule.append(key_transpose(key))
    for round_num in range(10):
        key = key_extend(key, round_num)
        key_schedule.append(key_transpose(key))
    return key_schedule


## 轮函数
def round_function_encrypt(text, key):
    '''加密轮函数'''
    text = subBytes(text)
    text = shiftRows(text)
    text = mixColumns(text)
    text = AddRoundKey(text, key)
    return text

def round_function_decrypt(text, key):
    '''解密轮函数'''
    text = inv_shiftRows(text)
    text = inv_subBytes(text)
    text = AddRoundKey(text, key)
    text = inv_mixColumns(text)
    return text


## AES函数
def AES_encrypt(text, key_schedule):
    '''AES加密函数'''
    text = text_to_array(text)
    text = AddRoundKey(text, key_schedule[0])
    for round_num in range(1, 10):
        text = round_function_encrypt(text, key_schedule[round_num])
    text = subBytes(text)
    text = shiftRows(text)
    text = AddRoundKey(text, key_schedule[10])
    text = array_to_hex(text)
    return text

def AES_decrypt(text, key_schedule):
    '''AES解密函数'''
    text = text_to_array(text)
    text = AddRoundKey(text, key_schedule[10])
    for round_num in range(1, 10):
        text = round_function_decrypt(text, key_schedule[10 - round_num])
    text = inv_shiftRows(text)
    text = inv_subBytes(text)
    text = AddRoundKey(text, key_schedule[0])
    text = array_to_hex(text)
    return text


# plain_text = 0x0123456789abcdeffedcba9876543210

seed_key = 0x01010101010101010101010101010101
# key_schedule = generate_key_schedule(seed_key)
key_schedule = [[[31, 14, 31, 14], [31, 14, 31, 14], [31, 14, 31, 14], [31, 14, 31, 14]], [[181, 187, 164, 170], [180, 186, 165, 171], [180, 186, 165, 171], [180, 186, 165, 171]], [[213, 110, 202, 96], [214, 108, 201, 98], [214, 108, 201, 98], [24, 162, 7, 172]], [[123, 21, 223, 191], [124, 16, 217, 187], [71, 43, 226, 128], [200, 106, 109, 193]], [[153, 140, 83, 236], [177, 161, 120, 195], [63, 20, 246, 118], [192, 170, 199, 6]], [[167, 43, 120, 148], [137, 40, 80, 147], [80, 68, 178, 196], [14, 164, 99, 101]], [[91, 112, 8, 156], [149, 189, 237, 126], [29, 89, 235, 47], [44, 136, 235, 142]], [[232, 152, 144, 12], [128, 61, 208, 174], [4, 93, 182, 153], [242, 122, 145, 31]], [[140, 20, 132, 136], [110, 83, 131, 45], [196, 153, 47, 182], [12, 118, 231, 248]], [[79, 91, 223, 87], [32, 115, 240, 221], [133, 28, 51, 133], [200, 190, 89, 161]], [[184, 227, 60, 107], [183, 196, 52, 233], [183, 171, 152, 29], [147, 45, 116, 213]]]

# print("明文:{:032x}\n密钥:{:032x}".format(plain_text, seed_key))

# cipher_text = AES_encrypt(plain_text, key_schedule)
# print("加密结果:{}".format(cipher_text))
# print("加密结果:{}".format("592b336fbd751d3348c82d8bbe401784"))

# decrypted_text = AES_decrypt(int(cipher_text, 16), key_schedule)
# print("解密结果:{}".format(decrypted_text))


# 读取文本
with open('input.txt', 'r') as file:
    text = file.read().strip()

# 判断、填充并分组
text_len = len(text)
if text_len % 32 > 0:
    text += ['0' for _ in range(32 - text_len)]
group_len = text_len // 32
text = [int(text[i * 32 : (i + 1) * 32], 16) for i in range(group_len)]

print("{:=^19}".format("AES加密程序"))
print("文件大小: {:.2f} MB".format(text_len / 2 / 1024 / 1024))

# 开始计时
start_time = time.time()

# 运行部分
result = []
for i in range(group_len):
    result.append(AES_encrypt(text[i], key_schedule))

# 计时结束
end_time = time.time()
execution_time = end_time - start_time
print("程序用时: {:.2f} s".format(execution_time))
print("平均速度: {:.1f} Kbps".format(text_len * 4 / 1024 / execution_time))

# 写入结果
with open('output.txt', 'w') as file:
    for i in range(group_len):
        file.write(result[i])

print("{:=^23}".format(''))
