# DDT.py

SBOX = {0x0, 0x4, 0x8, 0xf, 0x1, 0x5, 0xe, 0x9, 0x2, 0x7, 0xa, 0xc, 0xb, 0xd, 0x6, 0x3}

DDT = [[sum([SBOX[x] ^ SBOX[x ^ i] == j for x in range(16)]) for j in range(16)]for i in range(16)]