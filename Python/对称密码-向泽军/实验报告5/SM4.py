import time

sBox = [
        0xd6, 0x90, 0xe9, 0xfe, 0xcc, 0xe1, 0x3d, 0xb7, 0x16, 0xb6, 0x14, 0xc2, 0x28, 0xfb, 0x2c, 0x05,
        0x2b, 0x67, 0x9a, 0x76, 0x2a, 0xbe, 0x04, 0xc3, 0xaa, 0x44, 0x13, 0x26, 0x49, 0x86, 0x06, 0x99,
        0x9c, 0x42, 0x50, 0xf4, 0x91, 0xef, 0x98, 0x7a, 0x33, 0x54, 0x0b, 0x43, 0xed, 0xcf, 0xac, 0x62,
        0xe4, 0xb3, 0x1c, 0xa9, 0xc9, 0x08, 0xe8, 0x95, 0x80, 0xdf, 0x94, 0xfa, 0x75, 0x8f, 0x3f, 0xa6,
        0x47, 0x07, 0xa7, 0xfc, 0xf3, 0x73, 0x17, 0xba, 0x83, 0x59, 0x3c, 0x19, 0xe6, 0x85, 0x4f, 0xa8,
        0x68, 0x6b, 0x81, 0xb2, 0x71, 0x64, 0xda, 0x8b, 0xf8, 0xeb, 0x0f, 0x4b, 0x70, 0x56, 0x9d, 0x35,
        0x1e, 0x24, 0x0e, 0x5e, 0x63, 0x58, 0xd1, 0xa2, 0x25, 0x22, 0x7c, 0x3b, 0x01, 0x21, 0x78, 0x87,
        0xd4, 0x00, 0x46, 0x57, 0x9f, 0xd3, 0x27, 0x52, 0x4c, 0x36, 0x02, 0xe7, 0xa0, 0xc4, 0xc8, 0x9e,
        0xea, 0xbf, 0x8a, 0xd2, 0x40, 0xc7, 0x38, 0xb5, 0xa3, 0xf7, 0xf2, 0xce, 0xf9, 0x61, 0x15, 0xa1,
        0xe0, 0xae, 0x5d, 0xa4, 0x9b, 0x34, 0x1a, 0x55, 0xad, 0x93, 0x32, 0x30, 0xf5, 0x8c, 0xb1, 0xe3,
        0x1d, 0xf6, 0xe2, 0x2e, 0x82, 0x66, 0xca, 0x60, 0xc0, 0x29, 0x23, 0xab, 0x0d, 0x53, 0x4e, 0x6f,
        0xd5, 0xdb, 0x37, 0x45, 0xde, 0xfd, 0x8e, 0x2f, 0x03, 0xff, 0x6a, 0x72, 0x6d, 0x6c, 0x5b, 0x51,
        0x8d, 0x1b, 0xaf, 0x92, 0xbb, 0xdd, 0xbc, 0x7f, 0x11, 0xd9, 0x5c, 0x41, 0x1f, 0x10, 0x5a, 0xd8,
        0x0a, 0xc1, 0x31, 0x88, 0xa5, 0xcd, 0x7b, 0xbd, 0x2d, 0x74, 0xd0, 0x12, 0xb8, 0xe5, 0xb4, 0xb0,
        0x89, 0x69, 0x97, 0x4a, 0x0c, 0x96, 0x77, 0x7e, 0x65, 0xb9, 0xf1, 0x09, 0xc5, 0x6e, 0xc6, 0x84,
        0x18, 0xf0, 0x7d, 0xec, 0x3a, 0xdc, 0x4d, 0x20, 0x79, 0xee, 0x5f, 0x3e, 0xd7, 0xcb, 0x39, 0x48
]
CK = [
        0x00070e15, 0x1c232a31, 0x383f464d, 0x545b6269,
        0x70777e85, 0x8c939aa1, 0xa8afb6bd, 0xc4cbd2d9,
        0xe0e7eef5, 0xfc030a11, 0x181f262d, 0x343b4249,
        0x50575e65, 0x6c737a81, 0x888f969d, 0xa4abb2b9,
        0xc0c7ced5, 0xdce3eaf1, 0xf8ff060d, 0x141b2229,
        0x30373e45, 0x4c535a61, 0x686f767d, 0x848b9299,
        0xa0a7aeb5, 0xbcc3cad1, 0xd8dfe6ed, 0xf4fb0209,
        0x10171e25, 0x2c333a41, 0x484f565d, 0x646b7279
]
FK = [0xA3B1BAC6, 0x56AA3350, 0x677D9197, 0xB27022DC]


def int16array(p, len):
    if len == 32:
        return [p >> (96 - 32 * i) & 0xffffffff for i in range(4)]
    elif len == 8:
        return [p >> (24 - 8 * i) & 0xff for i in range(4)]


def array16str(p):
    str_16 = ""
    for i in range(4):
        str_16 += format(p[i], "08x")
    return str_16


def rotate(x, n):
    return x << n & 0xffffffff | x >> (32 - n)


def linear_trans(word, constant):
    result = word
    for i in constant:
        result ^= rotate(word, i)
    return result


def round_function(p, key_word):
    word = p[1] ^ p[2] ^ p[3] ^ key_word
    word = int16array(word, 8)
    word = sBox[word[0]] << 24 | sBox[word[1]] << 16 | sBox[word[2]] << 8 | sBox[word[3]]
    constant = [2, 10, 18, 24]
    word = linear_trans(word, constant)
    p[0] ^= word
    return [p[(i + 1) % 4] for i in range(4)]


def key_extend(k, n):
    word = k[1] ^ k[2] ^ k[3] ^ CK[n]
    word = int16array(word, 8)
    word = sBox[word[0]] << 24 | sBox[word[1]] << 16 | sBox[word[2]] << 8 | sBox[word[3]]
    constant = [13, 23]
    word = linear_trans(word, constant)
    k[0] ^= word
    return [k[(i + 1) % 4] for i in range(4)], k[0]


def key_schedule(K):
    subkeys = []
    k = int16array(K, 32)
    k = [k[i] ^ FK[i] for i in range(4)]
    for n in range(32):
        k, word = key_extend(k, n)
        subkeys.append(word)
    return subkeys


def encrypt(plaintext, keys):
    p = int16array(plaintext, 32)
    for i in range(32):
        p = round_function(p, keys[i])
    text = [p[4 - i - 1] for i in range(4)]
    text = array16str(text)
    return text


def decrypt(ciphertext, keys):
    c = int16array(ciphertext, 32)
    for i in range(32):
        c = round_function(c, keys[32 - i - 1])
    text = [c[4 - i - 1] for i in range(4)]
    text = array16str(text)
    return text


def read_input(input):
    text = open("./input.txt", "r").read()
    for i in range(2 ** 16):
        input.append(int(text[32 * i: 32 * (i + 1)], 16))


if __name__ == "__main__":
    plaintext = 0x0123456789abcdeffedcba9876543210
    Key = 0x01010101010101010101010101010101
    keys = key_schedule(Key)
    print("明文：0x{:032x}，密钥：0x{:032x}".format(plaintext, Key))
    ciphertext = encrypt(plaintext, keys)
    print("加密后密文：0x{}".format(ciphertext))
    decrypttext = decrypt(int(ciphertext, 16), keys)
    print("解密后明文：0x{}".format(decrypttext))
    input = []
    output = []
    read_input(input)
    t = time.perf_counter()
    for words in input:
        output.append(encrypt(words, keys))
    t = time.perf_counter() - t
    bps = 2 ** 16 * 128 / t / 1028 / 1028
    print("加密一串明文的时间为：{:.3f} s，加密速度为：{:.3f} Mbps".format(t, bps))