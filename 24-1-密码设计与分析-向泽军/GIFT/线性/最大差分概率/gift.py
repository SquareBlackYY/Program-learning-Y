from math import log2

import gurobipy as gp

SBOX = [0x1, 0xA, 0x4, 0xC, 0x6, 0xF, 0x3, 0x9, 0x2, 0xD, 0xB, 0x7, 0x5, 0x0, 0x8, 0xE]

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
        if LCT[i][j] != 0:
            points.append(
                list(map(int, list(format(i, "04b"))))
                + list(map(int, list(format(j, "04b"))))
                + (
                    list(map(int, list(format(1, "02b"))))
                    if abs(LCT[i][j]) == 2
                    else (
                        list(map(int, list(format(2, "02b"))))
                        if abs(LCT[i][j]) == 4
                        else (list(map(int, list(format(0, "02b")))))
                    )
                )
            )
# print(points)

# 将打印出来的point二维列表带入sagemath生成以下不等式系数矩阵
# points =
# poly = Polyhedron(vertices = points)
# for v in poly.inequality_generator():
# 	print(v)

# 凸闭包不等式系数矩阵
ineqs = [
    [0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 1],
    [0, -1, 0, 0, 0, 0, 0, 0, 1, 1, 0],
    [-1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0],
    [0, 0, -1, 0, 0, 0, 0, 0, 1, 1, 0],
    [0, 0, 0, -1, 0, 0, 0, 0, 1, 1, 0],
    [0, 0, -1, -1, -1, -1, 0, 0, 4, 3, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0],
    [0, 0, -1, 0, -1, 1, 0, 0, 1, 2, 0],
    [0, 0, 0, 0, -1, 0, 0, 0, 1, 1, 0],
    [0, -1, -1, 0, 0, -1, 0, -1, 3, 4, 0],
    [-1, -1, -1, 0, 0, 0, -1, 0, 3, 4, 0],
    [0, -2, -1, -1, -1, -1, -2, 0, 6, 7, 0],
    [0, -1, -1, 0, -1, 0, -1, 0, 3, 4, 0],
    [0, -1, 0, -1, 0, -1, 0, -1, 3, 4, 0],
    [0, -2, -2, -1, 1, 1, 0, 2, 3, 4, 0],
    [0, -1, 0, 0, 0, 1, 1, 1, 0, 1, 0],
    [0, 0, 0, -1, 1, 0, 1, 1, 0, 1, 0],
    [-1, -1, -1, -1, -1, 1, -1, 0, 4, 6, 0],
    [0, -1, -1, 0, -1, 0, 0, -1, 3, 4, 0],
    [-2, 0, -1, -1, -1, -1, 0, -2, 6, 7, 0],
    [-1, -1, -1, -1, -1, -1, -1, -2, 6, 8, 0],
    [-1, 0, 0, -1, 0, -1, 0, -1, 3, 4, 0],
    [0, -2, -1, -1, -1, -1, 0, -2, 6, 7, 0],
    [0, -1, -2, 0, -3, 1, -1, -1, 5, 8, 0],
    [0, -2, -1, -3, -3, -1, -2, -2, 10, 13, 0],
    [0, -1, -2, 0, -2, 0, -1, -1, 5, 7, 0],
    [-1, 0, -1, 0, -1, 0, 0, -1, 3, 4, 0],
    [-3, -3, -1, -1, -1, -1, -3, -2, 10, 14, 0],
    [-1, -1, -2, 0, -2, 0, -1, -2, 6, 9, 0],
    [-1, -1, -1, 0, -1, 0, -1, -1, 4, 6, 0],
    [-1, -2, 0, -1, -1, 0, -1, -1, 5, 7, 0],
    [0, 0, -1, 0, 0, 1, 1, 1, 0, 1, 0],
    [-1, -1, 0, -2, 0, -2, -1, -2, 6, 9, 0],
    [-1, -1, -1, 0, 0, -2, 0, -2, 5, 7, 0],
    [-1, -1, 0, 0, 0, -1, 0, -1, 3, 4, 0],
    [-1, 0, -1, 0, 0, -1, 0, -1, 3, 4, 0],
    [0, -2, 1, 0, 0, 1, 2, 2, 0, 1, 0],
    [0, -1, -2, 0, 0, -2, -1, -1, 5, 7, 0],
    [0, -1, -1, -1, -1, -1, -1, -1, 5, 6, 0],
    [-1, -1, 0, -1, 0, -1, -1, -1, 4, 6, 0],
    [0, 2, -3, -1, 1, 1, 4, 2, 0, 3, 0],
    [-2, -2, 0, 0, 0, -1, -1, -1, 5, 7, 0],
    [0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0],
    [0, -1, 0, -2, 0, -2, -1, -1, 5, 7, 0],
    [-1, -1, -2, 0, 0, -2, -1, -2, 6, 9, 0],
    [0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0],
    [0, 0, -1, -1, 1, 1, 2, 2, 0, 1, 0],
    [-4, 2, -1, -3, -1, -1, 2, -2, 8, 11, 0],
    [-1, 1, 0, -2, 1, -1, 1, 0, 2, 4, 0],
    [-1, 1, -1, 0, 0, 0, 1, 0, 1, 2, 0],
    [-2, 1, 0, -2, 0, -1, 1, -1, 4, 6, 0],
    [0, -1, 0, -1, 1, 1, 2, 2, 0, 1, 0],
    [0, 0, 0, -1, 1, -1, 0, 0, 1, 2, 0],
    [-2, -2, -1, 0, 0, -1, -2, -1, 6, 9, 0],
    [0, 0, 0, 0, 0, -1, 0, 0, 1, 1, 0],
    [0, 1, 1, -2, 2, -2, 1, 2, 0, 4, 0],
    [-2, 1, -2, 0, -2, 1, 1, -1, 4, 7, 0],
    [0, -1, -1, 0, 0, 0, -1, 1, 2, 3, 0],
    [-1, -1, 1, 0, 0, -1, 0, 0, 2, 3, 0],
    [0, 1, 0, -2, 2, -1, 2, 1, 0, 3, 0],
    [0, -1, -1, -2, 2, 2, 3, 4, 0, 2, 0],
    [0, -1, -1, 0, 0, 1, 0, 1, 1, 2, 0],
    [0, -2, 1, 1, -1, -2, 2, 0, 3, 4, 0],
    [0, 0, 1, 1, -1, -1, 2, 2, 0, 1, 0],
    [-1, 0, 1, 0, 0, -1, 0, 1, 1, 2, 0],
    [0, -1, 2, 0, 0, -1, 2, 1, 0, 2, 0],
    [0, 0, -1, 1, -1, 0, 0, 0, 1, 2, 0],
    [0, -1, 0, -1, 0, -1, -1, 0, 3, 4, 0],
    [-1, -1, 0, 0, 0, -1, -1, 0, 3, 4, 0],
    [0, -1, 2, 2, -2, -1, 4, 3, 0, 2, 0],
    [0, -1, 1, 0, 0, -1, 1, 0, 1, 2, 0],
    [0, 1, -2, 2, -3, 1, 1, 1, 1, 5, 0],
    [0, -1, -1, 0, 0, -1, -1, 0, 3, 4, 0],
    [0, -1, 0, 0, 0, -1, 1, -1, 2, 3, 0],
    [0, -1, 1, -3, 0, -2, -1, -1, 5, 8, 0],
    [0, -1, 1, 1, -1, 0, 2, 2, 0, 1, 0],
    [-1, 1, -2, 1, -2, 1, 1, 0, 2, 5, 0],
    [0, 1, -2, 0, 0, 1, 2, 1, 0, 2, 0],
    [0, 2, 1, 1, -1, -3, 2, 4, 0, 3, 0],
    [0, 0, -1, 1, -1, 1, 1, 1, 0, 2, 0],
    [0, 2, -1, -1, -1, -1, 2, 2, 2, 3, 0],
    [0, -1, -1, 0, 0, 2, 1, 2, 0, 2, 0],
    [0, -1, 1, 0, 0, 0, 1, 1, 0, 1, 0],
    [-1, -2, -1, 0, 0, -1, -2, 0, 5, 7, 0],
    [0, 0, 0, 1, -1, 0, 1, 1, 0, 1, 0],
    [0, 1, 1, 0, 0, -2, 1, 2, 0, 2, 0],
    [0, 1, 1, -3, 3, -2, 2, 2, 0, 5, 0],
    [-1, -1, 2, 0, 0, -1, 1, 1, 1, 3, 0],
    [-3, 1, 2, 2, -2, -3, 1, 5, 3, 6, 0],
    [0, 0, 1, -1, 1, -1, 1, 1, 0, 2, 0],
    [0, 1, -1, 1, -1, 0, 1, 1, 0, 2, 0],
    [0, 1, -1, 0, 0, 0, 1, 1, 0, 1, 0],
    [0, 0, 1, 0, 0, -1, 1, 1, 0, 1, 0],
    [-2, 0, 1, 1, -1, -2, 0, 2, 3, 4, 0],
    [0, 0, 1, -1, 0, -1, 0, 0, 1, 2, 0],
    [0, 1, -1, 2, -2, 0, 1, 2, 0, 3, 0],
    [0, 1, -2, 3, -3, 1, 2, 2, 0, 5, 0],
    [-1, 0, 0, -1, -1, 0, 1, -1, 3, 4, 0],
    [0, 1, 0, -1, 1, -1, 1, 1, 0, 2, 0],
    [0, 1, 0, 0, 0, -1, 1, 1, 0, 1, 0],
    [0, 1, 1, -3, 2, -2, 1, 1, 1, 5, 0],
    [0, 1, -2, 2, -2, 1, 2, 1, 0, 4, 0],
    [1, -1, 0, -1, 1, 1, 1, 1, 0, 1, 0],
    [1, -1, -2, 0, 0, 3, 1, 2, 0, 3, 0],
    [1, 0, 0, 0, 2, 2, 1, 1, -2, -2, 0],
    [1, 1, 0, -3, 3, -1, 2, 1, 0, 4, 0],
    [1, 0, -1, 0, 0, 1, 1, 0, 0, 1, 0],
    [0, 0, 0, 0, 1, 1, 1, 1, -1, -1, 0],
    [1, -1, -1, 0, 0, 2, 1, 1, 0, 2, 0],
    [1, -1, -1, 0, 0, 0, 1, -1, 2, 3, 0],
    [1, -2, -1, 0, 0, -1, 1, -2, 4, 6, 0],
    [-2, -1, -1, 0, 0, 0, -2, -1, 5, 7, 0],
    [-1, 0, -1, 0, 0, 0, -1, -1, 3, 4, 0],
    [1, 0, 0, -2, 2, 0, 1, 1, 0, 2, 0],
    [1, -1, 0, 0, 1, 0, 1, -1, 1, 2, 0],
    [-2, 0, -3, -1, -1, -1, -2, -4, 10, 13, 0],
    [0, 0, -1, -1, -1, -1, -2, -2, 6, 7, 0],
    [0, 1, -1, 0, 0, 1, 1, 0, 0, 1, 0],
    [-1, 0, 0, 0, 1, 1, -1, -1, 2, 2, 0],
    [-2, 2, -2, 0, -1, 1, 1, -1, 3, 6, 0],
    [1, -1, -1, 0, 1, 3, 1, 2, -1, 1, 0],
    [-1, 1, -1, 0, 0, 0, 0, -1, 2, 3, 0],
    [-1, 2, -1, 0, 0, 1, 1, -1, 1, 3, 0],
    [1, 1, -3, 0, 0, 2, 2, 1, 0, 3, 0],
    [0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0],
    [0, 0, 0, 1, 1, 0, 0, 0, 0, -1, 0],
    [1, 1, -1, 0, 2, 2, 2, 1, -2, -1, 0],
    [0, 2, -1, -1, 1, 1, 2, 0, 0, 1, 0],
    [-1, 2, 0, 0, 1, 2, 1, -1, 0, 1, 0],
    [1, -1, 0, 0, 1, 2, 1, 1, -1, 0, 0],
    [2, 1, 2, -5, 2, -3, -1, 1, 3, 9, 0],
    [2, -1, 1, 0, 2, 2, 1, 1, -2, -1, 0],
    [0, 2, 2, 3, 3, 2, 4, 4, -6, -5, 0],
    [2, -1, 1, 2, 0, 2, 1, 1, -2, 0, 0],
    [1, 6, 3, 2, -3, 1, -5, 5, 1, 5, 0],
    [0, 1, 0, 0, 1, 1, 1, 0, -1, -1, 0],
    [1, 0, -1, 0, -1, 0, -1, 0, 2, 3, 0],
    [1, 2, -3, 4, -5, 1, 1, 2, 1, 8, 0],
    [1, 1, 1, 0, 0, -1, 0, 0, 0, 1, 0],
    [1, 2, 0, 1, -1, 0, -1, 1, 0, 2, 0],
    [1, 0, 1, 0, 1, 2, 0, 0, -1, -2, 0],
    [1, -2, 2, 1, 0, 1, 2, 2, -1, 0, 0],
    [1, 3, 1, 1, -1, 0, -2, 2, 0, 2, 0],
    [2, 3, 0, 1, -1, -1, -1, 1, 0, 3, 0],
    [1, 2, 1, 3, 0, 0, 1, 2, -3, -1, 0],
    [4, 6, 1, 1, -1, -3, -2, 2, 0, 5, 0],
    [0, 1, 0, 1, 0, 0, 1, 1, -1, 0, 0],
    [0, 0, 2, 1, 0, 1, -1, -1, 1, 0, 0],
    [1, 2, 1, 2, 1, 0, 1, 2, -3, -2, 0],
    [2, 4, 1, 1, -1, -5, 2, 4, 0, 5, 0],
    [1, 1, -2, 3, -3, 1, 1, 1, 0, 5, 0],
    [1, 1, 0, 0, 1, 0, 0, 1, -1, 0, 0],
    [2, 3, 1, 0, 1, -1, -1, 1, -1, 2, 0],
    [1, 2, 1, 1, 0, 0, -1, 1, -1, 0, 0],
    [1, 2, -2, 3, -3, 0, 1, 2, 0, 5, 0],
    [2, 4, 1, 1, -1, -3, 0, 2, 0, 3, 0],
    [0, 0, -1, 0, -1, 0, -1, -1, 3, 4, 0],
    [2, 2, 4, 3, 1, 2, 0, 0, -4, -5, 0],
    [1, 2, -2, 3, -4, 1, 0, 1, 1, 6, 0],
    [2, 2, 0, 1, 1, 0, 0, 2, -2, -1, 0],
    [1, 1, 0, 1, 0, 0, 0, 1, -1, 0, 0],
    [2, 0, 1, 0, 2, 1, 1, 0, -2, -1, 0],
    [1, -1, -1, -1, 1, 2, 1, 2, 0, 2, 0],
    [1, 2, 1, 2, 0, 0, 0, 1, -2, -1, 0],
    [2, 1, 2, 0, 2, 1, 1, -1, -2, 0, 0],
    [2, 2, 0, 1, 5, 4, 4, 2, -6, -5, 0],
    [1, 1, -2, 2, -3, 1, 0, 1, 1, 5, 0],
    [1, 1, 1, 0, 1, 0, 0, 0, -1, 0, 0],
    [2, 4, 2, 3, 1, 0, 0, 2, -4, -3, 0],
    [2, 3, 1, 1, 0, -1, -1, 1, -1, 1, 0],
    [2, 1, -3, 2, -4, 2, -1, 2, 2, 8, 0],
    [1, 2, 1, 0, 0, -3, 1, 2, 0, 3, 0],
    [1, 2, 1, 1, 0, -1, 0, 1, -1, 0, 0],
    [0, 1, 0, 1, 0, 1, 1, 0, -1, 0, 0],
    [0, -1, 1, 1, 0, -1, 1, 0, 1, 1, 0],
    [1, 1, 1, 1, 0, 0, 0, 0, -1, -1, 0],
    [1, 1, -1, 1, -2, 1, -1, 1, 1, 4, 0],
    [-1, 1, 1, 1, -1, -1, 0, 2, 1, 2, 0],
    [2, 4, 4, 3, 1, 2, -2, 2, -4, -3, 0],
    [1, 2, 1, 0, 0, -2, 0, 1, 0, 2, 0],
    [2, 3, 1, 0, 0, -2, -1, 1, 0, 3, 0],
    [0, 0, 1, 0, 1, 0, 1, 1, -1, 0, 0],
    [1, 1, 0, 0, 0, -1, 0, 1, 0, 1, 0],
    [1, 1, 0, 1, -1, 0, 0, 0, 0, 1, 0],
    [2, 0, -1, -1, -1, -1, -2, 0, 4, 5, 0],
    [1, 2, -1, 2, -2, 0, 0, 1, 0, 3, 0],
    [2, 0, -1, -1, 1, 1, 0, 2, 0, 1, 0],
    [0, 0, 1, 1, 0, 1, -1, -1, 1, 1, 0],
    [2, 1, 2, -2, 2, -1, 1, -1, 0, 4, 0],
    [2, 0, 1, 3, 0, 2, 1, 1, -3, -1, 0],
    [3, -2, -2, -4, 4, 4, 3, 5, 0, 4, 0],
    [2, 1, 2, 3, 0, 1, 1, 0, -3, -2, 0],
    [1, 4, 2, 1, -2, 1, -4, 4, 1, 4, 0],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [1, 0, -1, 0, 0, -1, -1, 0, 2, 3, 0],
    [2, 1, 2, 2, 1, 1, 1, 0, -3, -3, 0],
    [1, 0, 1, 0, 1, 1, 0, 0, -1, -1, 0],
    [2, 3, -3, 4, -7, 2, -1, 1, 3, 11, 0],
    [0, 4, 2, 1, -2, 1, -3, 3, 1, 3, 0],
    [-2, 2, -1, -1, 0, 0, 1, -1, 3, 5, 0],
    [2, 0, 1, 1, 1, 1, 1, 0, -2, -2, 0],
    [2, 1, -3, 2, -5, 2, -1, 1, 3, 9, 0],
    [1, 1, 2, 2, 0, 1, 0, 0, -2, -2, 0],
    [2, 2, 2, 3, 3, 2, 2, 2, -6, -5, 0],
    [0, 1, 1, 1, 0, -1, 1, 2, -1, 0, 0],
    [0, 1, 2, 1, 0, 1, 0, 0, -1, -2, 0],
    [1, 1, 0, 0, 3, 2, 2, 1, -3, -2, 0],
    [2, 0, -2, 3, -3, 2, 1, 1, 0, 5, 0],
    [0, 0, 0, 0, 0, 0, 0, -1, 1, 1, 0],
    [1, 0, -2, 1, -2, 1, -1, 1, 2, 5, 0],
    [0, 1, 0, 2, 0, 1, 2, 1, -2, 0, 0],
    [1, 1, -2, -1, 1, 1, 2, 1, 0, 2, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0],
    [0, 1, 0, 0, 1, 0, 1, 1, -1, 0, 0],
    [-1, 0, -2, 0, 0, -1, -1, -2, 5, 7, 0],
    [0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0],
    [2, 2, -1, -1, -1, -1, 0, 2, 2, 3, 0],
    [0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 1, 1, 1, 0, 1, 0, 0, -1, -1, 0],
    [0, 1, 0, 1, -1, 0, 0, 1, 0, 1, 0],
    [0, 2, 1, 1, -1, 0, -1, 2, 0, 1, 0],
    [0, 2, 1, 0, -1, 1, -2, 2, 1, 2, 0],
    [0, 0, 0, 0, 0, 0, -1, 0, 1, 1, 0],
    [0, 0, 1, 0, 1, 2, -1, -1, 1, 0, 0],
    [3, -2, 1, 0, 3, 4, 2, 2, -3, -2, 0],
    [4, 2, 4, 3, 1, 2, 2, -2, -4, -3, 0],
    [1, 2, 2, 2, 0, 1, -1, 1, -2, -1, 0],
    [1, 2, 2, 2, 1, 1, 0, 1, -3, -3, 0],
    [1, 0, -1, 1, -1, 1, 0, 1, 0, 2, 0],
    [2, 1, 2, 0, 3, 1, 1, 0, -3, -1, 0],
    [1, 3, 3, 2, 0, 1, -2, 2, -2, -1, 0],
    [-2, 0, 1, -2, -2, 1, -2, 2, 6, 7, 0],
    [1, 2, 2, 3, 0, 1, 0, 1, -3, -2, 0],
    [1, 0, 0, 1, -1, 0, 1, 0, 0, 1, 0],
    [0, 1, 1, -1, 1, -1, 0, 1, 0, 2, 0],
    [2, 2, 0, 0, 0, -1, -1, 1, 0, 2, 0],
    [1, -3, 2, 1, -1, 1, 3, 3, 0, 2, 0],
    [1, 1, -1, 1, -1, 0, 0, 1, 0, 2, 0],
    [1, 1, 0, 0, 0, -1, -1, 0, 1, 2, 0],
    [-1, 1, 2, 2, 0, -1, 1, 3, -1, 0, 0],
    [2, 3, 1, -1, 1, -2, -1, 1, 0, 4, 0],
    [3, 2, -1, 0, 0, -1, -1, 2, 0, 3, 0],
    [-1, 2, -2, 1, -2, 2, 1, -1, 2, 6, 0],
    [1, 0, -1, -1, 1, 1, 1, 1, 0, 1, 0],
    [2, 1, -1, 0, 0, 0, -1, 2, 0, 2, 0],
    [2, -1, -1, 2, -2, 2, 1, 1, 0, 4, 0],
    [-1, 3, -1, -1, 1, 1, 2, -1, 1, 3, 0],
    [1, 1, 0, 0, 0, 0, -1, 1, 0, 1, 0],
    [1, 2, 1, 0, 1, 2, 2, -2, -1, 0, 0],
    [2, 0, -1, -1, 1, 1, 2, 0, 0, 1, 0],
    [2, 0, 1, 1, 2, 2, 1, 1, -3, -3, 0],
    [-1, 1, -2, 1, -2, 1, 0, -1, 3, 6, 0],
    [0, -1, 1, 0, 0, 1, -1, -1, 2, 2, 0],
    [0, 0, 0, 0, 1, 1, 0, 0, 0, -1, 0],
    [-2, 2, -3, 1, -3, 2, 1, -1, 4, 9, 0],
    [0, 1, -1, 1, -1, 1, 1, 0, 0, 2, 0],
    [0, 0, -1, 0, 0, -1, -1, -1, 3, 4, 0],
    [-1, -1, 1, 1, 0, -1, 0, 0, 2, 2, 0],
    [0, 1, 1, 1, 0, 0, 0, 1, -1, -1, 0],
    [-1, 0, 0, -2, 1, -1, -1, -1, 4, 6, 0],
    [1, 0, 0, 2, 0, 2, 1, 1, -2, 0, 0],
    [0, -1, -1, 0, 1, 1, 0, 1, 1, 1, 0],
    [1, 0, 0, 1, 0, 1, 0, 1, -1, 0, 0],
    [0, 0, 1, 1, 0, 0, 1, 1, -1, -1, 0],
    [0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [1, -1, 1, 1, -1, -1, 2, 0, 1, 2, 0],
    [1, -1, -2, 1, -1, 3, 1, 2, 0, 4, 0],
    [0, 2, 2, 1, 1, 2, 0, 0, -2, -3, 0],
    [-1, 0, -1, -1, 0, -1, -1, -2, 5, 7, 0],
    [1, -1, 0, 1, 0, 0, 1, -1, 1, 2, 0],
    [2, 1, 0, 2, -2, 1, 1, -1, 0, 3, 0],
    [1, 0, 1, 1, 0, 0, 1, 0, -1, -1, 0],
    [1, 2, 1, 0, 3, 0, 1, 2, -3, 0, 0],
    [1, 2, 2, -3, 2, -2, -1, 1, 1, 6, 0],
    [1, 2, 1, 0, 2, 0, 0, 1, -2, 0, 0],
    [1, 0, -1, 0, 0, 1, 0, 1, 0, 1, 0],
    [1, 2, 1, 0, 1, 0, -1, 1, -1, 1, 0],
    [-2, 4, -1, -2, 1, 1, 2, -2, 3, 6, 0],
    [0, 2, 2, 1, 0, 1, -1, 1, -1, -1, 0],
    [0, 0, 0, 1, 0, 1, 1, 1, -1, 0, 0],
    [1, 1, 0, 3, 0, 2, 2, 1, -3, 0, 0],
    [2, -1, 2, 2, -2, -1, 4, 1, 0, 2, 0],
    [1, -1, -1, 1, -1, 2, 1, 1, 0, 3, 0],
    [2, 0, -1, 2, -2, 1, 1, 0, 0, 3, 0],
    [2, 1, 0, -2, 1, -1, -1, 2, 1, 4, 0],
    [-1, 1, 0, 1, 0, 1, 0, -1, 1, 2, 0],
    [0, 1, 1, 2, 0, 0, 1, 2, -2, -1, 0],
    [2, 1, 2, 0, 0, -1, 1, -1, 0, 2, 0],
    [1, 1, 1, 1, 0, 1, -1, 1, -1, 0, 0],
    [1, 0, -1, 1, -2, 1, 0, 0, 1, 3, 0],
    [1, 2, 2, 1, 0, 1, -2, 2, -1, 0, 0],
    [1, 1, -1, 2, -3, 1, 0, 0, 1, 4, 0],
    [1, 1, 1, -3, 2, -2, 0, 1, 1, 5, 0],
    [-1, 2, 0, 1, 0, 2, 1, -1, 0, 2, 0],
    [1, 0, 0, -1, 1, 0, 1, 0, 0, 1, 0],
    [1, 0, -1, 0, 0, 0, -1, 1, 1, 2, 0],
    [1, -1, 0, -1, 1, -1, 1, -1, 2, 4, 0],
    [1, 1, 2, 0, 2, 1, 0, 0, -2, -1, 0],
    [2, 0, 1, 2, 0, 1, 1, 0, -2, -1, 0],
    [1, 1, 3, 2, 0, 1, 0, 0, -2, -3, 0],
    [1, 2, -1, 2, -3, 1, -1, 1, 1, 5, 0],
    [1, 2, 1, 0, 0, 1, 2, -2, 0, 1, 0],
    [1, 2, 1, -1, 1, -1, -1, 1, 0, 3, 0],
    [2, 1, 2, 2, 0, 1, 1, -1, -2, -1, 0],
    [1, 2, 1, -3, 3, -3, 1, 2, 0, 6, 0],
    [1, 1, 1, -1, 1, -1, 0, 0, 0, 2, 0],
    [2, 1, 1, 1, -1, 0, 1, -1, 0, 1, 0],
    [1, 1, 0, -1, 1, -1, 0, 1, 0, 2, 0],
    [0, 1, -1, 1, -2, 1, 0, 0, 1, 3, 0],
    [1, -1, -1, 1, -1, 0, 1, -1, 2, 4, 0],
    [2, 1, 0, -1, 0, -1, -1, 1, 1, 3, 0],
    [-1, 0, -3, 1, -3, 1, -1, -2, 6, 10, 0],
    [2, 1, 3, 2, 0, 1, 1, -1, -2, -2, 0],
    [1, 0, 0, -1, 0, -1, -1, 0, 2, 3, 0],
    [0, 1, 1, 0, 0, -1, 0, 1, 0, 1, 0],
    [2, 1, -1, 3, -3, 1, 1, 0, 0, 4, 0],
    [0, 2, 1, 1, -1, -1, 0, 2, 0, 1, 0],
    [4, 2, 4, 3, 0, 1, 2, -2, -3, -2, 0],
    [0, 1, 1, 0, 0, 0, -1, 1, 0, 1, 0],
    [1, 1, 1, 0, 3, 1, 1, 1, -3, -1, 0],
    [1, 1, 1, 3, 0, 1, 1, 1, -3, -1, 0],
    [0, 1, 1, 0, 0, 1, -1, 1, 0, 0, 0],
    [1, 1, 0, -1, 1, 1, 2, -1, 0, 1, 0],
    [1, 0, -2, 1, -3, 1, -1, 0, 3, 6, 0],
    [4, -2, 2, 1, 3, 4, 2, 2, -4, -3, 0],
    [2, 0, 2, 1, 1, 2, 0, 0, -2, -3, 0],
    [-1, 1, 0, 0, 1, 1, 0, -1, 1, 1, 0],
    [4, 2, -1, -1, -1, -1, -2, 2, 2, 5, 0],
    [1, -1, 1, 1, -1, 0, 2, 1, 0, 1, 0],
    [1, 0, 1, 1, 0, 1, 0, 0, -1, -1, 0],
    [0, 2, 1, 0, 1, 2, 1, -1, -1, -1, 0],
    [1, 2, 2, -4, 3, -3, 0, 1, 1, 7, 0],
    [2, 2, 2, 1, 1, 2, -2, 2, -2, -1, 0],
    [-1, 3, 1, 0, 2, 3, 1, -1, -1, -1, 0],
    [0, 1, 1, 0, 1, 0, 0, 1, -1, 0, 0],
    [1, 2, 1, -2, 2, -2, 0, 1, 0, 4, 0],
    [2, 0, 1, -2, 2, -1, 1, 0, 0, 3, 0],
    [1, -1, -1, 1, 0, 3, 1, 2, -1, 2, 0],
    [0, 1, -1, 0, 1, 1, 2, 1, -1, 0, 0],
    [1, 1, 1, -3, 3, -2, 1, 1, 0, 5, 0],
    [2, 1, 2, -3, 3, -2, 1, 0, 0, 5, 0],
    [1, 0, 0, -1, 1, 0, 0, 1, 0, 1, 0],
    [0, 1, 1, 0, 2, 0, 1, 2, -2, 0, 0],
    [0, 2, 2, 3, 1, 0, 2, 4, -4, -3, 0],
    [1, 0, 0, 0, 1, 1, 1, 0, -1, -1, 0],
    [2, 1, -1, 0, -1, 0, -1, 1, 1, 3, 0],
    [-1, 0, 2, -1, -1, -1, -1, 1, 3, 5, 0],
    [1, 0, 1, -3, 1, -2, -1, 0, 3, 6, 0],
    [1, 1, -2, 2, -4, 1, -1, 0, 3, 7, 0],
    [1, -1, -1, -1, 1, 1, 0, 2, 1, 2, 0],
    [0, 2, 1, -1, 0, 1, 2, -2, 1, 2, 0],
    [0, 1, 0, 0, 0, 1, 1, -1, 0, 1, 0],
    [1, 0, 0, -2, 1, -1, -1, 1, 2, 4, 0],
    [1, 1, -1, 1, -2, 0, -1, 0, 2, 4, 0],
    [1, 0, 1, 0, 1, 0, 1, 0, -1, 0, 0],
    [2, 3, -1, 2, -3, 0, -1, 1, 1, 5, 0],
    [0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0],
    [1, 2, 2, 0, 2, 1, -1, 1, -2, 0, 0],
    [0, 0, 1, -3, 1, -2, -1, -1, 4, 7, 0],
    [1, 0, 0, 0, 1, 1, 0, 1, -1, -1, 0],
    [0, 0, 1, 0, 0, 1, 0, 0, 0, -1, 0],
    [1, 0, 0, 1, 0, 1, 1, 0, -1, 0, 0],
    [0, 1, 1, 0, 0, 1, 1, -1, 0, 0, 0],
    [0, 0, 0, -1, 0, -1, -1, -1, 3, 4, 0],
    [0, 2, 0, 1, 1, 0, 2, 2, -2, -1, 0],
    [1, 1, -1, 0, 0, 0, 0, 1, 0, 1, 0],
    [-1, 0, 0, -3, 1, -2, -1, -2, 6, 9, 0],
    [-1, 1, 0, -2, 1, -1, 0, -1, 3, 5, 0],
    [-1, 0, 1, 1, 0, -1, 0, 1, 1, 1, 0],
    [0, 4, 1, -2, 1, 2, 3, -3, 1, 3, 0],
    [1, 2, 2, 0, 3, 1, 0, 1, -3, -1, 0],
    [0, 1, 0, 0, 2, 1, 2, 1, -2, -1, 0],
    [-1, 0, 0, -1, -1, 1, -1, 1, 3, 4, 0],
    [1, 1, 1, 0, 1, 1, -1, 1, -1, 0, 0],
    [1, 0, 1, 0, 0, -1, 1, 0, 0, 1, 0],
    [0, 1, 1, 3, 0, 1, 2, 2, -3, -1, 0],
    [0, 2, 0, 1, 3, 2, 4, 2, -4, -3, 0],
    [1, 1, 1, 0, 1, 2, -1, 1, -1, -1, 0],
    [0, 1, 1, 0, 3, 1, 2, 2, -3, -1, 0],
    [0, 1, 0, -1, 1, 0, 1, 0, 0, 1, 0],
    [-1, 0, 0, 1, 0, 1, -1, -1, 2, 3, 0],
    [-1, 1, 0, -1, 0, 0, 1, -1, 2, 3, 0],
    [2, -2, 0, 1, 1, 0, 2, -2, 2, 3, 0],
    [1, 0, 1, -2, 1, -1, 0, 0, 1, 3, 0],
    [1, 1, 2, -3, 2, -2, 0, 0, 1, 5, 0],
    [-1, 0, -2, 1, -2, 1, -1, -1, 4, 7, 0],
    [0, 1, 1, 0, 1, 2, 0, 0, -1, -2, 0],
    [0, 0, -2, 1, -3, 1, -1, -1, 4, 7, 0],
    [1, 3, 1, -1, 1, 2, 3, -3, 0, 2, 0],
    [0, 1, 1, 0, 1, 1, 0, 0, -1, -1, 0],
    [1, 0, 1, -1, 1, -1, 1, 0, 0, 2, 0],
    [-2, 2, 0, -3, 1, -1, 1, -1, 4, 7, 0],
    [0, 0, 1, 1, 0, 0, 0, 0, 0, -1, 0],
    [2, 0, 1, 0, 3, 2, 1, 1, -3, -2, 0],
    [1, 0, -1, 1, -1, 1, 1, 0, 0, 2, 0],
    [2, -1, 1, -2, 2, 0, 1, 1, 0, 3, 0],
    [-1, 2, 0, -2, 1, 0, 1, -1, 2, 4, 0],
    [-1, 3, 0, -2, 1, 1, 2, -2, 2, 4, 0],
    [2, 0, 1, -3, 3, -1, 1, 1, 0, 4, 0],
    [0, 2, 0, -1, 1, 1, 2, -1, 0, 1, 0],
    [2, -1, -1, -2, 2, 2, 1, 3, 0, 2, 0],
    [1, 0, -2, 0, 0, 2, 1, 1, 0, 2, 0],
    [1, 2, 2, -5, 4, -4, 1, 2, 1, 9, 0],
    [2, 1, 1, -4, 2, -2, -1, 2, 2, 7, 0],
    [1, 1, 1, -2, 1, -1, -1, 1, 1, 4, 0],
    [2, 3, 4, -7, 4, -5, -1, 1, 3, 13, 0],
    [1, 1, 2, -4, 2, -3, -1, 0, 3, 8, 0],
    [-1, 1, 1, -1, -1, 1, -2, 2, 3, 4, 0],
    [1, 2, 1, 1, 0, -2, 1, 2, -1, 1, 0],
    [0, 0, 1, 0, 1, 1, -1, -1, 1, 1, 0],
    [-1, 2, 0, -1, 1, 1, 1, -1, 1, 2, 0],
    [-1, 0, -2, 0, -1, 0, -1, -2, 5, 7, 0],
    [0, 0, 2, 1, 1, 2, -2, -2, 2, 1, 0],
    [1, 0, 2, 1, 0, 1, 0, 0, -1, -2, 0],
    [1, -1, 0, 1, 0, 2, 1, 1, -1, 1, 0],
    [-1, -1, 0, 0, 0, 0, -1, -1, 3, 4, 0],
    [1, 0, -2, 2, -2, 2, 1, 1, 0, 4, 0],
    [2, 0, 1, 1, -1, -1, 2, 0, 0, 1, 0],
    [1, 1, -3, 3, -3, 2, 2, 1, 0, 6, 0],
    [0, 1, 1, -2, 1, -1, 0, 0, 1, 3, 0],
    [2, 2, 1, 1, -1, -1, 0, 0, 0, 1, 0],
    [-4, -4, -1, -1, -2, -1, -3, -3, 13, 18, 0],
    [-3, -3, 0, -1, -1, -1, -2, -2, 9, 13, 0],
    [-2, -1, -1, -2, -2, 2, -1, 1, 6, 9, 0],
    [-2, -2, 1, -2, -2, 1, 0, 0, 6, 7, 0],
    [-2, -1, 3, -2, -2, -2, -1, 1, 6, 10, 0],
    [-2, 2, -1, -1, -1, -1, 2, 0, 4, 5, 0],
    [-1, 1, 0, -1, 0, -1, 1, 0, 2, 3, 0],
    [-1, 0, 1, 1, -1, -1, 1, 2, 1, 2, 0],
    [-1, -1, 2, 2, -2, -1, 3, 3, 1, 3, 0],
    [-2, -2, 1, 1, -1, -2, 0, 0, 5, 6, 0],
    [-4, -2, 1, -4, -4, 1, -2, 2, 12, 15, 0],
    [-1, -1, 0, -1, -1, 0, -1, 0, 4, 5, 0],
    [-1, -3, -1, -1, -1, -1, -3, 0, 8, 10, 0],
    [-3, -5, -1, -3, -3, -1, -5, -2, 16, 22, 0],
    [-1, -1, 1, -1, -1, -1, -1, 0, 4, 6, 0],
    [-1, -1, 2, -1, -1, -1, 0, 0, 3, 5, 0],
    [-1, -2, -1, -2, -2, -1, -1, -3, 9, 12, 0],
    [-3, -4, -2, -3, -5, -2, -1, -7, 19, 25, 0],
    [-1, -2, -2, -1, -3, 0, -1, -3, 9, 13, 0],
    [-1, -1, 0, -1, -1, 0, 0, -1, 4, 5, 0],
    [-2, -2, -1, -1, -1, -3, 0, -4, 10, 13, 0],
    [-1, -1, 0, -1, -1, -1, 0, -2, 5, 7, 0],
    [-2, -2, -1, -2, -3, -1, 0, -4, 11, 14, 0],
    [-3, -3, -2, -2, -4, -2, -1, -6, 16, 21, 0],
    [-1, -2, -1, -1, -2, -1, 0, -3, 8, 10, 0],
    [-2, -2, -1, -1, -2, -2, 0, -4, 10, 13, 0],
    [-2, -1, -1, -1, -2, -1, 0, -3, 8, 10, 0],
    [-1, -1, 0, -1, 0, -2, 0, -2, 5, 7, 0],
    [-1, -1, -1, 0, -1, -1, 0, -2, 5, 7, 0],
    [-1, -2, 0, -3, -1, -2, -1, -3, 9, 13, 0],
    [-1, -1, 0, -1, -1, 1, 0, 0, 3, 4, 0],
    [-3, -2, -2, -3, -3, 4, -1, 1, 9, 14, 0],
    [-1, -1, -1, -1, -1, 2, 0, 1, 3, 5, 0],
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

P = [
    (4 * (i // 16) + 16 * ((3 * ((i % 16) // 4) + (i % 4)) % 4) + (i % 4))
    for i in range(64)
]


def get_obj(round):
    return " + ".join(
        [
            " ".join(item)
            for item in list(
                zip(
                    ["4", "2"] * round * 16,
                    [f"u_{r}_{i}" for r in range(round) for i in range(32)],
                )
            )
        ]
    )


def get_vars(var, round, range):
    return [f"{var}_{round}_{i}" for i in range]


def get_sbox_constrains(round):
    s = ""
    for r in range(round):
        for i in range(16):
            x_in = get_vars("x", r, range(4 * i, 4 * (i + 1)))
            x_out = get_vars("x", r + 1, P[4 * i : 4 * (i + 1)])
            u = get_vars("u", r, range(2 * i, 2 * (i + 1)))
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
        [f"x_{r}_{i}" for r in range(round + 1) for i in range(64)]
        + [f"u_{r}_{i}" for r in range(round) for i in range(32)]
    )


ROUND = 30

for round in range(1, ROUND + 1):
    with open(f".\\lp\\gift_max_diff_prob_{round}.lp", "w") as f:
        f.write("Minimize\n")
        f.write(get_obj(round) + "\n")

        f.write("Subject To\n")
        f.write(" + ".join(get_vars("x", 0, range(64))) + " >= 1\n")  # 防止平凡解
        f.write(get_sbox_constrains(round))

        f.write("Binary\n")
        f.write(get_bin(round) + "\n")

        f.write("END")

    m = gp.read(
        f".\\lp\\gift_max_diff_prob_{round}.lp", env=gp.Env(params={"OutputFlag": 0})
    )
    m.optimize()
    if m.Status == 2:
        print(f"{round}\t{m.ObjVal}")
