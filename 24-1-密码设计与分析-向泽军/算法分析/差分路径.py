import gurobipy as gp

SBOX = [0x0, 0x4, 0x8, 0xF, 0x1, 0x5, 0xE, 0x9, 0x2, 0x7, 0xA, 0xC, 0xB, 0xD, 0x6, 0x3]

L0 = L0_INV = [
    0b0000100010001000,
    0b0000010001000100,
    0b0000001000100010,
    0b0000000100010001,
    0b1000000010001000,
    0b0100000001000100,
    0b0010000000100010,
    0b0001000000010001,
    0b1000100000001000,
    0b0100010000000100,
    0b0010001000000010,
    0b0001000100000001,
    0b1000100010000000,
    0b0100010001000000,
    0b0010001000100000,
    0b0001000100010000,
]

L1 = [
    0b1100000000010000,
    0b0110000000001000,
    0b0011000000000100,
    0b0001100000000010,
    0b0000110000000001,
    0b0000011010000000,
    0b0000001101000000,
    0b1000000100100000,
    0b1000000000011000,
    0b0100000000001100,
    0b0010000000000110,
    0b0001000000000011,
    0b0000100010000001,
    0b0000010011000000,
    0b0000001001100000,
    0b0000000100110000,
]

L1_INV = [
    0b0000001100000010,
    0b1000000100000001,
    0b1100000010000000,
    0b0110000001000000,
    0b0011000000100000,
    0b0001100000010000,
    0b0000110000001000,
    0b0000011000000100,
    0b0001000000011000,
    0b0000100000001100,
    0b0000010000000110,
    0b0000001000000011,
    0b0000000110000001,
    0b1000000011000000,
    0b0100000001100000,
    0b0010000000110000,
]

L2 = [
    0b0000110000000001,
    0b0000011010000000,
    0b0000001101000000,
    0b1000000100100000,
    0b1100000000010000,
    0b0110000000001000,
    0b0011000000000100,
    0b0001100000000010,
    0b0000100010000001,
    0b0000010011000000,
    0b0000001001100000,
    0b0000000100110000,
    0b1000000000011000,
    0b0100000000001100,
    0b0010000000000110,
    0b0001000000000011,
]

L2_INV = [
    0b0011000000100000,
    0b0001100000010000,
    0b0000110000001000,
    0b0000011000000100,
    0b0000001100000010,
    0b1000000100000001,
    0b1100000010000000,
    0b0110000001000000,
    0b0000000110000001,
    0b1000000011000000,
    0b0100000001100000,
    0b0010000000110000,
    0b0001000000011000,
    0b0000100000001100,
    0b0000010000000110,
    0b0000001000000011,
]

L3 = L3_INV = [
    0b1000100000001000,
    0b0100010000000100,
    0b0010001000000010,
    0b0001000100000001,
    0b1000100010000000,
    0b0100010001000000,
    0b0010001000100000,
    0b0001000100010000,
    0b0000100010001000,
    0b0000010001000100,
    0b0000001000100010,
    0b0000000100010001,
    0b1000000010001000,
    0b0100000001000100,
    0b0010000000100010,
    0b0001000000010001,
]

L0_Table = [[] for _ in range(16)]
L1_Table = [[] for _ in range(16)]
L2_Table = [[] for _ in range(16)]
L3_Table = [[] for _ in range(16)]

for i in range(16):
    for j in range(16):
        if (L0[i] >> j) & 1 == 1:
            L0_Table[i].append(j)
        if (L1[i] >> j) & 1 == 1:
            L1_Table[i].append(j)
        if (L2[i] >> j) & 1 == 1:
            L2_Table[i].append(j)
        if (L3[i] >> j) & 1 == 1:
            L3_Table[i].append(j)

DDT = [
    [sum([SBOX[x] ^ SBOX[x ^ i] == j for x in range(16)]) for j in range(16)]
    for i in range(16)
]

LCT = [[-8 for i in range(16)] for j in range(16)]
for in_mask in range(16):
    for out_mask in range(16):
        for x in range(16):
            if (
                format((in_mask & x), "04b").count("1")
                + format((out_mask & SBOX[x]), "04b").count("1")
            ) % 2 == 0:
                LCT[in_mask][out_mask] += 1

# 点集
points = []
for i in range(16):
    for j in range(16):
        if DDT[i][j] != 0:
            points.append(
                list(map(int, list(format(i, "04b"))))
                + list(map(int, list(format(j, "04b"))))
                + (
                    list(map(int, list(format(2, "02b"))))
                    if DDT[i][j] == 2
                    else (
                        list(map(int, list(format(1, "02b"))))
                        if DDT[i][j] == 4
                        else list(map(int, list(format(0, "02b"))))
                    )
                )
            )
# print(points)

ineqs = [
    [0, 0, 0, 0, 0, 0, 0, 0, -1, -1, +1],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 1, +0],
    [-1, -1, 0, 0, 0, 0, 0, 0, 2, 1, +0],
    [-1, 0, 0, 0, 0, 0, 0, 0, 1, 1, +0],
    [0, -1, 0, 0, 0, 0, 0, 0, 1, 1, +0],
    [0, 0, -1, 0, 0, 0, 0, 0, 1, 1, +0],
    [0, 0, 0, -1, 0, 0, 0, 0, 1, 1, +0],
    [0, 0, 0, 0, 0, 0, 0, -1, 1, 1, +0],
    [0, 0, 0, 0, -1, 0, 0, 0, 1, 1, +0],
    [0, -1, 0, 0, -1, 0, 0, 0, 2, 1, +0],
    [-1, -1, 0, 0, -1, 0, 0, -1, 4, 2, +0],
    [-1, -1, 0, -1, -1, 0, 0, 0, 4, 2, +0],
    [-1, -2, 0, -1, -1, 0, 0, 1, 4, 2, +0],
    [-1, 0, 0, 0, -1, 0, 0, -1, 3, 2, +0],
    [-1, 0, 0, -1, -1, 0, 0, 0, 3, 2, +0],
    [-1, -1, 0, -1, -1, 0, 0, 1, 3, 2, +0],
    [-1, -1, 0, -1, -1, -1, 0, 1, 4, 2, +0],
    [0, 0, 0, 0, 0, -1, 0, 0, 1, 1, +0],
    [-1, 0, 0, 0, 0, -1, 0, 0, 2, 1, +0],
    [-1, 0, 0, 0, -1, -1, 0, -1, 4, 2, +0],
    [-1, 0, 0, -1, -1, -1, 0, 0, 4, 2, +0],
    [-2, -1, -1, 1, -1, -1, 0, -2, 7, 4, +0],
    [-2, 0, -1, 1, -1, -2, 0, -2, 7, 4, +0],
    [-2, 0, -1, 1, -1, -1, 0, -2, 6, 4, +0],
    [-1, 0, 0, 1, -1, -1, 0, -1, 3, 2, +0],
    [-1, -1, 0, 1, -1, -1, 0, -1, 4, 2, +0],
    [-1, 0, 0, 1, -1, -2, 0, -1, 4, 2, +0],
    [0, 0, 0, 0, -1, -1, 0, 0, 2, 1, +0],
    [0, -1, -1, 1, -1, -1, -1, -1, 5, 4, +0],
    [1, -1, -1, 0, -1, 0, -1, -1, 4, 4, +0],
    [1, -1, -1, 0, 0, 0, -1, -1, 3, 4, +0],
    [-1, -1, 0, -2, -2, 0, -1, 1, 6, 4, +0],
    [-1, -2, 0, -2, -2, 0, -1, 1, 7, 4, +0],
    [-1, -1, 0, -2, -2, -1, -1, 1, 7, 4, +0],
    [-1, 0, -1, -1, 1, -1, -1, 0, 4, 4, +0],
    [0, 0, 0, 0, 0, 0, -1, 0, 1, 1, +0],
    [0, 0, -1, -1, 1, -1, -1, 0, 3, 4, +0],
    [-1, -1, -1, -1, 0, -1, -1, 1, 5, 4, +0],
    [0, -2, -1, -2, -1, -1, 1, 1, 6, 4, +0],
    [-5, -1, -3, 3, -2, -3, 1, -6, 17, 10, +0],
    [0, 0, -1, 0, -1, -1, 1, -1, 4, 2, +0],
    [-3, -1, -1, 1, 0, -1, 1, -2, 7, 4, +0],
    [-2, -1, -1, 1, 0, -1, 1, -1, 5, 3, +0],
    [-2, 0, -1, 1, -1, -2, 1, -3, 8, 4, +0],
    [-1, 0, -1, 1, -1, -2, 1, -2, 6, 3, +0],
    [-1, 0, 0, 1, -1, -1, 1, -2, 4, 2, +0],
    [0, -2, -1, -1, -1, 0, 1, 1, 4, 3, +0],
    [-1, 0, -1, 0, 0, -1, 1, -1, 4, 2, +0],
    [0, -1, -1, -1, -1, 0, 1, 1, 3, 3, +0],
    [0, 0, -1, 0, 0, -1, 1, -1, 3, 2, +0],
    [-1, 0, -1, 0, -1, -1, 1, -2, 6, 3, +0],
    [-1, 0, -1, -1, -1, -1, 1, -1, 6, 3, +0],
    [-3, -1, -1, 3, -2, -4, 1, -4, 12, 6, +0],
    [-1, -1, -1, 1, 0, -1, 1, -1, 4, 3, +0],
    [-1, 0, 0, 0, 0, 0, 1, -1, 2, 1, +0],
    [0, -1, -1, -1, -1, -1, 1, 1, 4, 3, +0],
    [0, -1, -1, -2, -1, -2, 1, 1, 6, 4, +0],
    [-1, 0, 0, -1, -1, 0, 1, -1, 4, 2, +0],
    [1, 0, -1, 0, 1, 0, -1, 1, 1, 1, +0],
    [1, 1, 0, 0, 1, 1, 0, 0, -2, -1, +0],
    [3, 1, -2, 0, 3, 1, -1, 1, -1, 0, +0],
    [-1, -1, 1, -2, -1, 0, 0, 1, 4, 2, +0],
    [0, 1, 1, 1, 0, -1, 0, 0, 0, 0, +0],
    [1, 1, -1, -1, 2, 0, 0, 0, 0, 1, +0],
    [0, 1, 0, 0, 1, 2, 1, 0, -2, -1, +0],
    [-3, -1, 1, -1, 1, 0, 1, 0, 4, 2, +0],
    [-1, 0, 0, 0, 1, 0, 1, 0, 1, 0, +0],
    [0, -1, -1, -1, 1, -1, -1, 2, 4, 3, +0],
    [3, 0, 0, 1, 2, 1, -2, -1, 0, 1, +0],
    [1, 0, 0, 0, 0, 1, -1, 1, 1, 0, +0],
    [1, 0, 0, 0, 1, 1, -1, 0, 0, 0, +0],
    [0, 0, 0, 0, 1, 0, 0, 0, 0, 0, +0],
    [0, -1, 0, 0, 0, 1, 0, 1, 1, 0, +0],
    [-1, 1, -1, -1, 1, 1, 2, 0, 1, 1, +0],
    [-1, 0, 0, -1, 1, 2, 2, 1, 0, 0, +0],
    [1, -1, -1, 1, 0, 0, -1, -1, 3, 3, +0],
    [0, -1, -1, 1, 1, 0, 1, 0, 1, 2, +0],
    [1, 0, 0, 0, 1, 0, 0, 0, -1, 0, +0],
    [0, 2, -1, 1, 1, 3, 1, -1, -1, 0, +0],
    [1, -1, 0, 0, 0, 1, -1, -1, 2, 2, +0],
    [0, 0, -1, 1, 0, -1, -1, 0, 3, 2, +0],
    [0, -1, -1, 1, 0, 0, -1, 0, 3, 2, +0],
    [0, -1, 0, -1, 0, 0, 0, 1, 2, 1, +0],
    [1, 0, 0, 0, 0, 1, -1, -1, 1, 2, +0],
    [0, 1, -1, 0, 1, 0, 1, 0, 1, 0, +0],
    [3, 2, -1, 1, 4, 2, 0, 2, -4, -3, +0],
    [0, 0, -1, 0, 0, -1, -1, 1, 3, 2, +0],
    [0, -2, -1, 2, -1, -1, -1, -1, 6, 4, +0],
    [0, 1, -2, -1, 1, -1, 1, 0, 3, 2, +0],
    [-1, 1, 0, 1, -1, 0, 0, 0, 1, 2, +0],
    [1, 0, 0, 0, 1, 1, 1, 1, -2, -1, +0],
    [-1, 1, -1, 1, -1, -4, -1, 0, 7, 4, +0],
    [0, 1, 2, 2, -2, -1, 1, -3, 4, 2, +0],
    [-1, -4, -1, 0, -1, 1, -1, 1, 7, 4, +0],
    [-2, -3, 1, -6, -5, -1, -3, 3, 17, 10, +0],
    [1, 1, 1, 0, 0, 1, -1, -1, 0, 1, +0],
    [2, -1, -1, 0, 0, 1, -2, -2, 4, 5, +0],
    [0, -1, 1, -2, -3, -1, -1, 1, 7, 4, +0],
    [0, -1, 1, -1, -2, -1, -1, 1, 5, 3, +0],
    [1, 1, 1, 1, 1, 1, 1, 1, -3, -2, +0],
    [0, 1, -1, -1, 1, 0, 1, 0, 1, 1, +0],
    [1, -1, 1, 0, 1, 2, 1, 3, -1, -2, +0],
    [-2, -1, 1, -1, 1, 0, 1, 1, 3, 1, +0],
    [1, -1, 1, 0, 0, 1, -2, -1, 3, 2, +0],
    [1, -2, 2, 0, 1, 2, 2, 4, -1, -2, +0],
    [1, -1, 1, 0, 1, 2, 2, 3, -2, -2, +0],
    [-2, -4, 1, -6, -5, -1, -3, 3, 18, 10, +0],
    [-1, -1, 1, 1, 0, -1, -1, -1, 4, 3, +0],
    [1, 0, 1, 0, 1, 2, 2, 2, -3, -2, +0],
    [0, 1, 0, 0, 1, 1, 1, 0, -1, -1, +0],
    [0, -1, 0, 1, 0, 0, -1, -1, 3, 2, +0],
    [0, 0, 1, 1, -1, -1, 0, -1, 3, 1, +0],
    [0, -1, -1, 2, -1, -2, -1, -1, 6, 4, +0],
    [0, 1, 1, -1, 0, 1, 0, 0, 0, 0, +0],
    [1, 1, 0, 0, 0, 1, -1, 1, 0, 0, +0],
    [1, 1, 0, 1, 1, 0, 0, 0, -1, -1, +0],
    [-1, 0, 1, 1, 1, -1, -1, 0, 2, 2, +0],
    [0, -1, 1, -1, -1, -1, -1, 1, 4, 3, +0],
    [1, 0, 1, 0, 0, 1, -1, 0, 1, 0, +0],
    [0, 0, 1, 0, -1, 0, 0, -1, 2, 1, +0],
    [0, 0, 1, -1, -1, 0, 0, 0, 2, 1, +0],
    [0, 1, -1, 1, 1, -1, 0, -1, 3, 1, +0],
    [1, -1, -1, 2, 0, -1, -1, -1, 4, 3, +0],
    [1, 2, 3, 3, 0, -1, 1, -1, -1, -1, +0],
    [2, 0, 1, 0, -3, -1, -1, 1, 4, 2, +0],
    [0, -2, 1, -2, -2, 0, -1, 1, 7, 3, +0],
    [2, -1, 1, -1, 0, 3, -2, 2, 4, 0, +0],
    [1, 2, 2, 3, 1, -1, 1, 0, -2, -2, +0],
    [-1, 0, 1, -2, -1, 0, 0, 0, 4, 2, +0],
    [0, 2, -1, 1, 1, -1, 0, 0, 2, 0, +0],
    [2, 0, 0, 0, 1, 1, -1, -1, 0, 1, +0],
    [1, 2, 1, 3, 1, -1, 1, 0, -1, -2, +0],
    [-1, 1, -1, -1, 1, 0, 2, 0, 2, 1, +0],
    [1, 2, 2, 1, -1, 0, 0, -1, 0, 0, +0],
    [0, 1, -2, 2, 2, 1, 0, 1, 1, 0, +0],
    [1, 1, 1, 1, 0, 0, 0, 0, -1, -1, +0],
    [1, 1, -2, 2, 2, 1, -1, 1, 1, 0, +0],
    [1, 1, 1, 0, 0, 1, 0, 0, -1, -1, +0],
    [1, 0, 0, 0, -1, 0, -1, 1, 2, 1, +0],
    [0, 1, 0, 0, 0, 0, 0, 0, 0, 0, +0],
    [-1, -1, 1, 0, 1, 0, 1, 1, 2, 0, +0],
    [1, 0, 1, 0, -1, 0, 0, 0, 1, 0, +0],
    [2, 0, 1, 0, 0, 2, -1, 1, 1, -1, +0],
    [0, 1, 1, 1, 0, 0, 1, -1, 0, 0, +0],
    [1, 0, 1, 0, 0, 1, -1, -1, 1, 1, +0],
    [3, 1, -2, 1, 3, 1, -2, 1, 0, 0, +0],
    [0, 1, -2, 0, 0, -1, 1, -1, 4, 2, +0],
    [-2, -3, 1, -4, -3, -1, -1, 3, 11, 6, +0],
    [0, 0, -1, -1, 0, -1, 1, 0, 3, 2, +0],
    [2, 1, -1, 1, 3, 1, -2, 0, 0, 0, +0],
    [-1, -2, 1, -2, -2, -1, -1, 2, 7, 4, +0],
    [0, 0, -1, 1, 1, 0, -1, 0, 2, 1, +0],
    [-1, -1, 1, 1, 0, -2, -1, -2, 6, 4, +0],
    [0, 0, 1, 2, -1, -2, 1, -1, 3, 1, +0],
    [2, -1, -1, 2, 1, 0, -2, -1, 3, 3, +0],
    [1, 2, 2, 1, 0, 0, 0, 0, -2, -1, +0],
    [0, 0, 0, 0, 0, 1, 1, 1, -1, 0, +0],
    [2, 1, 1, 2, 2, 1, 1, 2, -4, -3, +0],
    [0, 0, 0, 1, 0, 0, 0, 0, 0, 0, +0],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, +0],
    [0, 0, 0, 0, 1, 2, 2, 1, -2, -1, +0],
    [0, 1, -1, 1, 1, -1, -1, 0, 3, 1, +0],
    [0, 0, 0, -1, -1, 0, -1, 1, 3, 2, +0],
    [1, 1, -1, 1, 2, 0, -1, 0, 1, 0, +0],
    [2, 0, 0, 0, 1, 2, -1, 1, 0, -1, +0],
    [1, -2, 0, 1, 0, 1, -2, -1, 4, 3, +0],
    [0, 3, -2, 2, 2, -1, 1, -1, 4, 0, +0],
    [-1, -1, -1, 1, 0, 0, -1, -1, 5, 3, +0],
    [0, 0, 0, 1, -1, -1, 1, -1, 3, 1, +0],
    [-2, -4, 1, -3, -1, 0, 1, 4, 8, 3, +0],
    [1, -2, -1, 2, 0, 0, -2, -1, 5, 4, +0],
    [1, 2, 1, 0, 0, 1, 0, 0, -2, -1, +0],
    [3, 1, -1, 1, 3, 1, -2, 0, -1, 0, +0],
    [4, 2, 0, 2, 3, 2, -1, 1, -4, -3, +0],
    [2, 1, -2, -1, 3, 0, 0, 1, 0, 1, +0],
    [2, 1, -1, 1, 2, 0, -1, 0, 0, 0, +0],
    [0, 0, 0, 0, 0, 0, 1, 0, 0, 0, +0],
    [1, 0, 1, -1, -3, -1, -1, 1, 5, 3, +0],
    [0, -1, -1, 0, 0, 0, -1, 1, 3, 2, +0],
    [-1, -3, 0, 0, -1, 1, 0, 1, 4, 2, +0],
    [1, 0, 1, 0, -3, -1, 1, -1, 4, 2, +0],
    [0, 1, 0, 2, 0, -1, 1, -1, 2, 0, +0],
    [-1, 0, 0, 0, -1, 0, 1, -2, 4, 2, +0],
    [-2, 1, -1, 1, -1, -3, 0, -2, 8, 4, +0],
    [1, 2, 3, 3, 1, -1, -1, 1, -2, 0, +0],
    [1, 1, 2, 0, -1, 1, -1, -1, 1, 1, +0],
    [-1, -2, 1, -1, 0, 0, 1, 2, 3, 1, +0],
    [1, 2, 2, 1, -1, 1, -1, -1, 0, 1, +0],
    [1, 0, 1, 1, -2, -1, 1, -1, 3, 1, +0],
    [1, -1, -1, -2, 0, -1, 1, 1, 4, 3, +0],
    [-1, 0, 1, 1, 0, -1, -1, -1, 3, 3, +0],
    [0, 0, 0, 0, -1, 0, 1, -1, 2, 1, +0],
    [-1, -2, 1, -2, -1, 0, -1, 1, 6, 3, +0],
    [2, -1, -1, -2, 1, 0, 1, 2, 2, 2, +0],
    [1, 1, 1, 2, 0, -1, 1, 0, 0, -1, +0],
    [-1, -1, -1, 1, 0, -2, 1, -2, 6, 4, +0],
    [-1, 1, 0, 1, -1, 1, 0, 1, 0, 2, +0],
    [1, 2, 1, 0, 1, 2, 1, 0, -3, -2, +0],
    [0, 1, -1, 0, 0, -1, 0, 0, 2, 1, +0],
    [0, 1, 1, 1, 1, 1, 1, 0, -2, -1, +0],
    [0, 0, 1, 0, 0, 0, 0, 0, 0, 0, +0],
    [1, 2, 3, 2, 0, -1, -1, 1, -1, 0, +0],
    [0, 1, -1, 1, 1, 1, 0, 0, 0, 0, +0],
    [1, 1, 1, 0, 0, 1, 1, 1, -2, -1, +0],
    [0, -1, 1, 0, 0, 0, -1, -1, 3, 2, +0],
    [0, -2, 1, -2, -3, -1, -1, 1, 8, 4, +0],
    [0, 1, 0, 1, 0, -1, 0, 0, 1, 0, +0],
    [1, 0, 1, 2, -2, -2, 1, -1, 4, 1, +0],
    [1, 0, 1, 1, -1, -1, 1, 0, 2, 0, +0],
    [1, 0, 0, 0, 1, 1, 0, 1, -1, -1, +0],
    [1, -1, -1, -1, 0, 0, 1, 1, 2, 2, +0],
    [1, 0, -1, -1, 1, 0, 0, 1, 1, 1, +0],
    [1, -1, 1, 0, -1, 0, 1, 1, 2, 0, +0],
    [0, -1, 1, -1, 0, 0, -1, 0, 3, 2, +0],
    [0, 1, 1, 2, 0, -2, 1, 0, 1, 0, +0],
    [1, 1, 1, 0, -1, 0, 0, 0, 0, 0, +0],
    [1, 0, 1, 0, -2, -1, -1, 1, 3, 2, +0],
    [-1, -1, 1, -3, -2, 0, -1, 1, 7, 4, +0],
    [-3, -1, -1, 1, 1, 0, 1, -1, 5, 3, +0],
    [-2, -1, -1, 1, 1, 0, 1, 0, 3, 2, +0],
    [-1, -2, 1, -3, -2, 0, -1, 1, 8, 4, +0],
    [0, -2, 1, -1, 1, 2, 3, 4, 0, -1, +0],
    [-1, -2, 1, 1, 0, -1, -1, -2, 6, 4, +0],
    [1, -1, 1, -1, 1, -1, 1, -1, 2, 4, +0],
    [0, -1, 1, -1, 0, 1, -2, 0, 4, 2, +0],
    [2, 1, -2, 2, 2, 1, -2, 2, 1, 0, +0],
    [-1, 1, -1, -1, 2, 3, 3, 1, -1, 0, +0],
    [0, 1, 2, 2, 1, -1, -1, 0, 0, 1, +0],
    [0, 1, 2, 2, -1, -1, 1, -2, 2, 1, +0],
    [1, -1, -2, 1, 1, 1, -2, 1, 4, 2, +0],
    [-1, -1, 1, -1, 0, 0, 0, 1, 3, 1, +0],
    [1, -1, 1, 0, 0, 1, 1, 2, 0, -1, +0],
    [2, 1, 0, 1, 0, 1, -2, 2, 1, 0, +0],
    [0, -1, 1, 0, -1, 0, -1, -1, 4, 2, +0],
    [1, 2, 1, 4, 0, -2, 1, -1, 2, -1, +0],
    [1, -1, 2, 0, -1, 1, -2, -1, 4, 2, +0],
    [-1, 1, -1, 1, 0, -2, 0, -1, 5, 2, +0],
    [0, -1, 1, 1, 1, -1, -1, -2, 4, 3, +0],
    [1, 0, 1, 1, 1, 1, 1, 2, -2, -2, +0],
    [0, 0, -1, -1, -1, -1, -1, 1, 5, 3, +0],
    [0, -1, 0, 0, 0, 1, -1, 0, 2, 1, +0],
    [-1, -1, 1, -1, 0, 0, -1, 0, 4, 2, +0],
    [-1, 0, 1, -1, -1, 0, 0, -1, 4, 2, +0],
    [-1, -3, 0, -2, -2, 1, -1, 1, 8, 4, +0],
    [0, 0, -2, -1, -1, -2, -1, 2, 7, 4, +0],
    [-1, 0, 0, 0, -1, 1, 0, 1, 1, 2, +0],
    [-1, 0, 1, 1, 0, -2, -1, -1, 4, 3, +0],
    [1, 0, 1, 0, 0, 1, 0, 1, 0, -1, +0],
    [1, 3, 4, 3, 0, -1, 1, -1, -2, -1, +0],
    [1, 0, -1, 0, 0, 0, -1, 1, 2, 1, +0],
    [1, 2, 1, 1, 1, 1, 1, 0, -3, -2, +0],
    [1, 0, 2, 0, -1, 1, -1, -1, 2, 1, +0],
    [2, 1, 0, 1, 2, 1, 0, 1, -3, -2, +0],
    [1, 2, 2, 4, 1, -2, 2, 0, -1, -2, +0],
    [1, -1, -1, 0, -1, 0, 1, 1, 2, 2, +0],
    [-1, 0, 1, -1, 0, 0, 0, 0, 2, 1, +0],
    [0, 1, 1, 1, -1, 0, 0, -1, 1, 1, +0],
    [2, 0, -1, 0, 2, 1, -1, 1, 0, 0, +0],
    [0, 1, 1, 1, 0, 0, 0, 0, -1, 0, +0],
    [0, -1, 0, 0, 0, 1, 1, 1, 0, 0, +0],
    [1, 1, 1, 0, 0, 0, 0, 0, -1, 0, +0],
    [2, 3, 3, 1, -1, 1, -1, -1, -1, 0, +0],
    [-1, -1, 1, -1, -1, 0, -1, -1, 6, 3, +0],
    [0, -2, 1, 0, 0, 1, 1, 2, 1, 0, +0],
    [-1, -1, 1, -2, -1, 0, -1, 0, 6, 3, +0],
    [-2, -4, 1, -4, -3, -1, -1, 3, 12, 6, +0],
    [-1, 0, -1, 1, 1, 0, 0, 0, 2, 1, +0],
    [-1, 0, -1, 1, 0, 0, 0, -1, 3, 2, +0],
    [-3, -1, -1, 3, -2, -3, 1, -4, 11, 6, +0],
    [0, 1, -1, 1, 0, -2, -1, 0, 4, 2, +0],
    [-1, 1, -2, -1, 1, -1, 2, 0, 4, 2, +0],
    [-1, -3, 1, -3, -2, 0, -1, 2, 9, 4, +0],
    [0, 0, 0, 0, 0, 1, 0, 0, 0, 0, +0],
    [-2, 1, -2, 2, 0, -3, 1, -3, 10, 4, +0],
    [0, -1, 1, -1, -1, 0, -1, 0, 4, 2, +0],
    [0, -1, 0, -1, -1, 0, -1, 1, 4, 2, +0],
    [0, -1, 1, 0, 1, 1, 1, 2, 0, -1, +0],
    [-1, 0, -1, 1, 0, -1, 0, -1, 4, 2, +0],
    [0, -3, 1, -3, -2, 1, -2, 2, 10, 4, +0],
    [0, -2, 1, -2, -1, -1, -1, 1, 6, 4, +0],
    [0, 0, 1, -1, 0, 1, 1, 1, 0, 0, +0],
    [0, 0, -2, -1, 1, -2, -1, 2, 5, 4, +0],
    [1, 0, -1, 1, 1, 1, -1, 1, 1, 0, +0],
    [2, 1, -1, 1, 1, 1, -2, 2, 1, 0, +0],
    [3, 1, 1, 2, 2, 1, -2, -1, -1, 0, +0],
    [0, 1, -1, 1, 1, 0, 0, 0, 1, 0, +0],
    [2, 1, 1, 1, 1, 1, -1, -1, -1, 0, +0],
    [1, 0, -1, 1, 1, 0, -1, 0, 1, 1, +0],
    [1, 0, 1, 0, 0, -1, -1, 1, 1, 2, +0],
    [2, 1, 0, 1, 2, 1, -1, 0, -2, -1, +0],
    [1, 2, 3, 4, 0, -2, 1, -1, 0, -1, +0],
    [0, -2, 1, -1, 1, 2, 1, 4, 2, -1, +0],
    [0, 0, 0, 0, 0, 0, 0, 0, 1, 0, +0],
    [0, -1, 1, -1, 1, 0, 1, 0, 1, 2, +0],
    [0, -1, 1, -1, 1, 2, 3, 3, -1, -1, +0],
    [-2, -1, 1, -3, 0, 1, 2, 2, 4, 2, +0],
    [2, 1, -1, 1, 2, 1, -1, 1, -1, -1, +0],
    [-2, 0, -1, 1, 0, -2, 1, -2, 7, 3, +0],
    [0, 1, -2, -1, 1, -2, 0, 1, 4, 3, +0],
    [0, -2, -1, 0, 0, 1, -1, 1, 4, 2, +0],
    [1, 1, -1, 1, 1, 0, -1, 1, 1, 0, +0],
    [-1, 0, 1, 2, -2, -2, 1, -3, 6, 3, +0],
    [1, -1, 0, 0, 0, 2, -1, 1, 2, 0, +0],
    [0, 1, -1, -1, 1, 1, 1, 0, 0, 1, +0],
    [0, 0, 0, 0, 1, 1, 1, 0, -1, 0, +0],
    [0, 1, -1, -1, 1, -1, 0, 0, 2, 2, +0],
    [1, 1, -1, 0, 1, 0, 0, 0, 0, 0, +0],
    [0, 0, 1, 1, 0, -1, 1, 0, 1, 0, +0],
    [1, -1, -1, 1, 1, 0, 1, 1, 0, 2, +0],
    [0, -1, 1, -1, 1, 3, 4, 3, -2, -1, +0],
    [1, 0, 1, 2, 2, -1, -1, -2, 2, 2, +0],
    [0, -1, -1, 1, 1, 2, 3, 2, -1, 0, +0],
    [-1, 0, 2, 2, 1, -2, -2, -1, 4, 4, +0],
    [-2, 0, -1, 2, -1, -3, 1, -3, 9, 4, +0],
    [-3, -1, -1, 1, 2, 0, 1, 0, 4, 2, +0],
    [-1, 0, 1, 1, 1, -1, 1, 0, 2, 0, +0],
    [1, 1, 1, 2, 1, 0, 1, 1, -2, -2, +0],
    [0, 0, 1, 1, -1, -1, 1, -1, 2, 1, +0],
    [1, 1, 1, 1, 1, 0, 0, 0, -2, -1, +0],
    [1, -1, 0, -1, 0, 1, -1, 1, 3, 1, +0],
    [2, 0, -1, 0, 1, 1, -1, 1, 1, 0, +0],
    [3, 1, -2, 0, 2, 1, -1, 1, 0, 0, +0],
    [-1, -1, 1, -1, 0, 0, 1, 1, 2, 1, +0],
    [-1, -1, -1, -1, 0, -2, -1, 2, 6, 4, +0],
    [0, -2, 0, -1, -1, 1, -1, 1, 5, 2, +0],
    [1, -1, -1, 0, 0, 1, -1, 1, 3, 1, +0],
    [0, -1, 1, -1, 0, 1, 0, 2, 2, 0, +0],
    [-1, 0, 0, 2, -1, -2, 1, -2, 5, 2, +0],
    [1, 1, -2, 1, 1, -1, -2, 1, 4, 2, +0],
    [2, 1, -1, 0, 2, 1, -1, 0, -1, 0, +0],
    [1, 3, 1, -1, 0, 2, -1, 1, -1, 0, +0],
    [0, 2, -1, 1, 2, 0, 1, 0, 1, -1, +0],
    [0, 0, 1, 1, 1, -1, -1, -1, 2, 2, +0],
    [-1, -2, -1, 2, 0, 0, -2, -1, 7, 4, +0],
    [0, 0, 0, 1, 0, -1, 0, -1, 2, 1, +0],
    [-1, 0, 1, 4, -2, -4, 1, -3, 8, 3, +0],
    [1, 2, -1, 1, 2, 0, 0, 0, 0, -1, +0],
    [0, -2, 1, -1, 0, 1, 1, 3, 2, 0, +0],
    [0, -1, 1, 0, 0, 0, 1, 1, 1, 0, +0],
    [0, 1, 0, 0, 0, 1, 1, -1, 0, 0, +0],
    [-1, 0, 0, -1, 0, 1, 1, 1, 1, 1, +0],
    [2, 1, -1, 0, 2, 1, 0, 1, -2, -1, +0],
    [0, 1, 0, 0, 0, 1, 0, 0, -1, 0, +0],
    [1, 0, -2, 1, 1, 0, -2, 1, 3, 2, +0],
    [0, 1, 1, 2, 1, -1, 1, 0, 0, -1, +0],
    [1, -1, -1, 1, 1, 2, 3, 3, -2, 0, +0],
    [0, 1, 1, 3, 0, -2, 1, -1, 2, 0, +0],
    [1, 0, -2, -1, 2, -1, -1, 2, 3, 3, +0],
    [-2, -1, -1, 2, -1, -2, 1, -2, 7, 4, +0],
    [1, 2, 2, 2, 1, 0, 1, 0, -3, -2, +0],
    [-1, 1, -1, -1, 1, 2, 2, 1, 0, 1, +0],
    [1, 0, 1, 0, 0, -1, 1, -1, 1, 2, +0],
    [0, 0, -1, -1, 0, -1, 0, 1, 3, 2, +0],
    [1, 1, -1, -1, 2, 1, 1, 1, -1, 0, +0],
    [0, 0, 1, 1, -2, -1, 1, -2, 4, 2, +0],
    [1, 1, 1, 0, 1, 2, 1, 1, -3, -2, +0],
    [1, -1, -1, 0, 0, 0, 1, 1, 1, 2, +0],
    [0, 0, 1, 1, 1, -1, -1, 0, 1, 2, +0],
    [-1, 0, 0, -1, 0, 0, 1, 0, 2, 1, +0],
    [0, 1, -1, -1, 1, 0, 0, 0, 1, 2, +0],
    [-2, -2, 1, -1, 1, 0, 1, 2, 4, 1, +0],
    [1, 0, 0, 1, 1, 0, -1, -1, 1, 1, +0],
    [1, 0, 1, 1, 1, -1, -1, 1, 0, 2, +0],
    [0, 1, 0, 1, 1, 0, 1, 0, 0, -1, +0],
    [-1, 1, 0, 1, -1, -3, 0, 0, 4, 2, +0],
    [-1, 0, 0, 0, 1, 1, 1, 0, 0, 0, +0],
    [-3, -1, -1, 1, 0, -2, 1, -2, 8, 4, +0],
    [-1, -2, -1, -1, 0, -1, -1, 2, 6, 4, +0],
    [0, 1, -2, -2, 2, -1, -1, 0, 4, 5, +0],
    [1, -1, -1, 0, 0, 1, 2, 2, 0, 1, +0],
    [-1, -2, 1, -2, -1, 0, 0, 2, 5, 2, +0],
    [-1, 0, -1, -1, 0, -1, 1, 0, 4, 2, +0],
    [0, 0, 0, 0, 1, 1, 1, 1, -1, -1, +0],
    [0, 0, 0, 0, 0, 0, 0, 1, 0, 0, +0],
    [2, 1, -2, -1, 3, 1, 1, 2, -1, 0, +0],
    [0, 0, -1, -1, 1, -1, -1, 1, 3, 3, +0],
    [-2, -2, 1, -3, -1, 0, 1, 2, 6, 3, +0],
    [-1, -1, 0, -1, 0, 0, 1, 1, 3, 1, +0],
    [-1, -1, 1, -2, 0, 1, 2, 2, 2, 1, +0],
    [-2, -1, 1, -2, 0, 0, 1, 1, 4, 2, +0],
    [1, -2, -2, -1, -1, 0, 2, 2, 4, 4, +0],
    [-5, -1, -3, 3, -2, -4, 1, -6, 18, 10, +0],
    [-2, 0, -1, 1, -1, -1, 1, -3, 7, 4, +0],
    [1, -1, -1, -1, 1, -1, -1, -1, 4, 6, +0],
]


# 判断是否满足凸闭包不等式
def evaluate(point, ineq):
    point.append(1)
    if sum([point[i] * ineq[i] for i in range(len(ineq))]) >= 0:
        return True
    else:
        return False


# 不合法点集
cpoints = []
for i in range(1 << 10):
    tmp = list(map(int, list(format(i, "010b"))))
    if tmp not in points:
        cpoints.append(tmp)

# 所取的不等式系数矩阵
res = []
while len(cpoints) > 0:
    counter = -1
    index = -1
    for i in range(len(ineqs)):
        t = 0
        for p in cpoints:
            if not evaluate(p, ineqs[i]):
                t += 1
        if t > counter:
            index = i
            counter = t
    res.append(ineqs[index])
    ineqs.pop(index)
    new_cpoints = []
    for p in cpoints:
        if evaluate(p, res[-1]):
            new_cpoints.append(p)
    cpoints = new_cpoints
# print(res)
# print(len(res))


def get_obj(round):
    return " + ".join(
        [
            " ".join(item)
            for item in list(
                zip(
                    ["3", "2"] * round * 16,
                    [f"u_{r}_{i}" for r in range(round) for i in range(32)],
                )
            )
        ]
    )


def get_vars(var, round, range):
    return [f"{var}_{round}_{i}" for i in range]


def get_constrains(round):
    s = ""
    for r in range(round):
        if not r == 0:
            for j in range(16):
                s += f"xout_{r - 1}_{L0_Table[j][0]} + xout_{r - 1}_{L0_Table[j][1]} + xout_{r - 1}_{L0_Table[j][2]} + xin_{r}_{4 * (15 - j) + 3} - 2 k_{r - 1}_{4 * (15 - j) + 3} = 0\n"
                s += f"xout_{r - 1}_{L1_Table[j][0]} + xout_{r - 1}_{L1_Table[j][1]} + xout_{r - 1}_{L1_Table[j][2]} + xin_{r}_{4 * (15 - j) + 2} - 2 k_{r - 1}_{4 * (15 - j) + 2} = 0\n"
                s += f"xout_{r - 1}_{L2_Table[j][0]} + xout_{r - 1}_{L2_Table[j][1]} + xout_{r - 1}_{L2_Table[j][2]} + xin_{r}_{4 * (15 - j) + 1} - 2 k_{r - 1}_{4 * (15 - j) + 1} = 0\n"
                s += f"xout_{r - 1}_{L3_Table[j][0]} + xout_{r - 1}_{L3_Table[j][1]} + xout_{r - 1}_{L3_Table[j][2]} + xin_{r}_{4 * (15 - j)} - 2 k_{r - 1}_{4 * (15 - j)} = 0\n"
        for i in range(16):
            x_in = get_vars("xin", r, range(4 * i, 4 * (i + 1)))
            x_out = get_vars("xout", r, range(4 * i, 4 * (i + 1)))
            u = get_vars("u", r, range(2 * i, 2 * (i + 1)))
            k = get_vars("k", r, range(4 * i, 4 * (i + 1)))
            for res_item in res:
                s += (
                    " + ".join(
                        [
                            " ".join(item)
                            for item in list(
                                zip(list(map(str, res_item[0:-1])), x_in + x_out + u)
                            )
                        ]
                    ).replace("+ -", "- ")
                    + f" >= {-res_item[-1]}\n"
                )
    return s


def get_bin(round):
    return "\n".join(
        [f"xin_{r}_{i}" for r in range(round) for i in range(64)]
        + [f"xout_{r}_{i}" for r in range(round) for i in range(64)]
        + [f"u_{r}_{i}" for r in range(round) for i in range(32)]
    )

def get_int(round):
    return "\n".join(
        [f"k_{r}_{i}" for r in range(round - 1) for i in range(64)]
    )

ROUND = 9

for round in range(1, ROUND + 1):
    with open(f".\\diff\\smallPride_max_diff_prob_{round}.lp", "w") as f:
        f.write("Minimize\n")
        f.write(get_obj(round) + "\n")

        f.write("Subject To\n")
        f.write(" + ".join(get_vars("xin", 0, range(64))) + " >= 1\n")  # 防止平凡解
        f.write(get_constrains(round))

        f.write("Binary\n")
        f.write(get_bin(round) + "\n")

        f.write("Integer\n")
        f.write(get_int(round) + "\n")

        f.write("END")

    m = gp.read(
        f".\\diff\\smallPride_max_diff_prob_{round}.lp", env=gp.Env(params={"OutputFlag": 0})
    )
    m.optimize()
    if m.Status == 2:
        print(f"{round}\t{m.ObjVal}")
