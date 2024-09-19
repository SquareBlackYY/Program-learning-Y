# import gurobipy as gp

SBOX = [0x0, 0x4, 0x8, 0xF, 0x1, 0x5, 0xe, 0x9, 0x2, 0x7, 0xa, 0xc, 0xb, 0xd, 0x6, 0x3]

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

for item in DDT:
    print(item)

print()

for item in LCT:
    print(item)