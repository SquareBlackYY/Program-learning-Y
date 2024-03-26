from gurobipy import *

ROUND = 4

def get_state_vars(r):
    return [["_".join(["a", str(r), str(i), str(j)]) for j in range(4)] for i in range(4)]

def get_tmp_vars(r):
    return ["_".join(["d", str(r), str(i)]) for i in range(4)]

def shift_row(s):
    return [[s[i][(i + j) % 4] for j in range(4)] for i in range(4)]

def constrains(in_vars, out_vars, tmp_var):
    s = ""
    s += " + ".join(in_vars + out_vars) + " - 5 " + tmp_var + " >= 0\n"
    for i in range(4):
        s += tmp_var + " - " + in_vars[i] + " >= 0\n"
    for i in range(4):
        s += tmp_var + " - " + out_vars[i] + " >= 0\n"
    return s

def get_obj():
    return " + ".join(["_".join(["a", str(r), str(i), str(j)])for r in range(ROUND)for i in range(4)for j in range(4)])

with open("aes.lp", "w") as f:
    f.write("Minimize\n")
    f.write(get_obj() + "\n")
    f.write("Subject To\n")
    f.write(" + ".join((["_".join(["a_0", str(i), str(j)]) for i in range(4) for j in range(4)])) + " >= 1\n")
    state_in = get_state_vars(0)
    for i in range(ROUND):
        state_in = shift_row(state_in)
        state_out = get_state_vars(i + 1)
        state_in = list(zip(*state_in))
        state_out = list(zip(*state_out))
        t = get_tmp_vars(i)
        for j in range(4):
            f.write(constrains(state_in[j], state_out[j], t[j]))
        state_in = list(zip(*state_out))
    f.write("Binary\n")
    for r in range(ROUND + 1):
        for i in range(4):
            for j in range(4):
                f.write("_".join(["a", str(r), str(i), str(j)]) + "\n")
    for r in range(ROUND):
        for i in range(4):
            f.write("_".join(["d", str(r), str(i)]) + "\n")
    f.write("END")

read("aes.lp").optimize()