
ROUND = 4

def get_state_vars(r):
    return ["_".join(["b", str(r), str(i)]) for i in range(4)]

def get_obj():
    return " + ".join(["_".join(["b", str(r), str(i)])for r in range(ROUND)for i in range(4)])

