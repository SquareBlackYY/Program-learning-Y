# import gurobipy as gp
import sagemath as sm

SBOX = [0x1, 0xa, 0x4, 0xc, 0x6, 0xf, 0x3, 0x9, 0x2, 0xd, 0xb, 0x7, 0x5, 0x0, 0x8, 0xe]

DDT = [[sum([SBOX[x] ^ SBOX[x ^ i] == j for x in range(16)]) for j in range(16)] for i in range(16)]

points = []
for i in range(16):
    for j in range(16):
        if DDT[i][j] != 0:
            points.append(list(map(int, list(format(i, "04b")))) + list(map(int, list(format(j, "04b")))))

P = [(4 * (i // 16) + 16 * ((3 * (i % 16) // 4 + (i % 4)) % 4) + (i % 4)) for i in range(64)]

def get_obj(round):
    return " + ".join([f"A_{r}_{i}" for r in range(round) for i in range(16)])

def get_vars(var, round, range):
    return [f"{var}_{round}_{i}" for i in range]

def get_sbox_constrains(round):
    s = ""
    for r in range(round):
        for i in range(16):
            x_in = get_vars('x', r, range(4 * i, 4 * (i + 1)))
            inv_index = [P.index(value) for value in range(4 * i, 4 * (i + 1))]
            x_out = get_vars('x', r, inv_index)
            s += " + ".join([f"4 {var}" for var in x_in]) + " - " + " - ".join([var for var in x_out]) + " >= 0\n"
            s += " + ".join([f"4 {var}" for var in x_out]) + " - " + " - ".join([var for var in x_in]) + " >= 0\n"
            for item in list(zip(get_vars('A', r, [i]) * 4, x_in)):
                s += " - ".join(item) + " >= 0\n"
    return s

def get_bin(round):
    return '\n'.join([f"A_{r}_{i}" for r in range(round) for i in range(16)])

ROUND = 1
with open("gift_min_active_sbox.lp", "w") as f:

    f.write("Minimize\n")
    f.write(get_obj(ROUND) + '\n')

    f.write("Subject To\n")
    f.write(" + ".join(get_vars('x', 0, range(64))) + " >= 0\n") # 防止平凡解
    f.write(get_sbox_constrains(ROUND))

    f.write("Binary\n")
    f.write(get_bin(ROUND) + '\n')

    f.write("END")