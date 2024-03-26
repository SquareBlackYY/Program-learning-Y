# L函数还没写

from gurobipy import *

ROUND = 1
LFSR_B = []
OBJ = []
ALL_B = []
TMP = []
ALL_TMP = []
SBOX = [2, 7, 16, 29]
XOR = [[31, 32, 0], [32, 2, 35], [33, 7, 36], [35, 36], [37, 16, 32], [38, 29, 33], [2, 6, 3], [7, 15, 16], [16, 28, 17]]
CONSTRAINS = []

# 变量名生成
def get_b(i):
    s = "_".join(["b", str(i)])
    ALL_B.append(s)
    return s

def get_tmp(i):
    s = "_".join(["d", str(i)])
    ALL_TMP.append(s)
    return s

# 变量名迭代器
def get_b_generator():
    i = 0
    while True:
        yield get_b(i)
        i += 1

def get_tmp_generator():
    i = 0
    while True:
        yield get_tmp(i)
        i += 1

# 获取要加入目标函数的变量
def get_obj():
    OBJ.extend([LFSR_B[i] for i in SBOX])

with open("enocoro.lp", "w") as f:

    # 创建变量名迭代器
    b_iter = get_b_generator()
    tmp_iter = get_tmp_generator()

    # 填充寄存器
    for _ in range(35):
        LFSR_B.append(next(b_iter))
    get_obj()

    # 轮函数
    for _ in range(ROUND):
        # 创建下一状态的寄存器，前31位右移，保留第32位及以后的元素
        LFSR_B_next = [""] + LFSR_B[:31] + LFSR_B[32:]
        # 计算差分传播
        for xor in XOR:
            # 三叉树
            if len(xor) == 3:
                if xor[0] >= 37:
                    new_b = next(b_iter)
                    new_tmp = next(tmp_iter)
                    CONSTRAINS.append(LFSR_B_next[xor[0]] + " + " + LFSR_B[xor[1]] + " + " + new_b + " - 2 " + new_tmp + " >= 0")
                    CONSTRAINS.append(new_tmp + " - " + LFSR_B_next[xor[0]] + " >= 0")
                    CONSTRAINS.append(new_tmp + " - " + LFSR_B[xor[1]] + " >= 0")
                    CONSTRAINS.append(new_tmp + " - " + new_b + " >= 0")
                    LFSR_B_next[xor[2]] = new_b
                elif xor[2] <= 33:
                    new_b = next(b_iter)
                    new_tmp = next(tmp_iter)
                    CONSTRAINS.append(LFSR_B[xor[0]] + " + " + LFSR_B[xor[1]] + " + " + new_b + " - 2 " + new_tmp + " >= 0")
                    CONSTRAINS.append(new_tmp + " - " + LFSR_B[xor[0]] + " >= 0")
                    CONSTRAINS.append(new_tmp + " - " + LFSR_B[xor[1]] + " >= 0")
                    CONSTRAINS.append(new_tmp + " - " + new_b + " >= 0")
                    LFSR_B_next[xor[2]] = new_b
                else:
                    new_b = next(b_iter)
                    new_tmp = next(tmp_iter)
                    CONSTRAINS.append(LFSR_B[xor[0]] + " + " + LFSR_B[xor[1]] + " + " + new_b + " - 2 " + new_tmp + " >= 0")
                    CONSTRAINS.append(new_tmp + " - " + LFSR_B[xor[0]] + " >= 0")
                    CONSTRAINS.append(new_tmp + " - " + LFSR_B[xor[1]] + " >= 0")
                    CONSTRAINS.append(new_tmp + " - " + new_b + " >= 0")
                    LFSR_B_next.append(new_b)
            # 线性变换
            else:
                new_b1 = next(b_iter)
                new_b2 = next(b_iter)
                new_tmp = next(tmp_iter)
                CONSTRAINS.append(LFSR_B_next[xor[0]] + " + " + LFSR_B_next[xor[1]] + " + " + new_b1 + " + " + new_b2 + " - 3 " + new_tmp + " >= 0")
                CONSTRAINS.append(new_tmp + " - " + LFSR_B_next[xor[0]] + " >= 0")
                CONSTRAINS.append(new_tmp + " - " + LFSR_B_next[xor[1]] + " >= 0")
                CONSTRAINS.append(new_tmp + " - " + new_b1 + " >= 0")
                CONSTRAINS.append(new_tmp + " - " + new_b2 + " >= 0")
                LFSR_B_next.append(new_b1)
                LFSR_B_next.append(new_b2)
        # 一轮结束后刷新寄存器状态，截取前34位元素
        LFSR_B = LFSR_B_next.copy()[:35]
        get_obj()

    # 写入目标函数
    f.write("Minimize\n")
    f.write(" + ".join(OBJ) + "\n")

    # 写入限制条件
    f.write("Subject To\n")
    f.writelines(i + "\n" for i in CONSTRAINS)

    # 变量限制
    f.write("Binary\n")
    f.writelines(i + "\n" for i in ALL_B)
    f.writelines(i + "\n" for i in ALL_TMP)

    f.write("END")
    