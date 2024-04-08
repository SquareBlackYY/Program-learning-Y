sbox = [0xC, 0x5, 0x6, 0xB, 0x9, 0x0, 0xA, 0xD, 0x3, 0x3, 0xF, 0x8, 0x4, 0x7, 0x1, 0x2]

ineqs = [
    [-1, 0, 0, 0, 0, 0, 0, 0],
    [0, -1, 0, 0, 0, 0, 0, 0],
    [0, 0, -1, 0, 0, 0, 0, 0],
    [-1, -2, -2, -1, -1, 2, -2, 0],
    [0, 0, 0, -1, 0, 0, 0, 0],
    [-2, -2, 1, -1, -2, -1, 1, 0],
    [-1, -1, -1, -1, 0, 1, -1, 0],
    [-1, -1, -1, 0, -1, 1, -1, 0],
    [0, 0, 0, 0, -1, 0, 0, 0],
    [0, -1, -1, -1, -1, 1, -1, 0],
    [-1, 1, -1, -1, 0, -1, -1, 0],
    [0, 0, 0, 0, 0, -1, 0, 0],
    [-1, -1, 1, 0, -1, -1, 0, 0],
    [-1, -2, 2, -1, -1, -2, -1, 0],
    [-1, 0, -1, -1, 1, 0, -1, 0],
    [-1, -1, 1, -1, 0, -1, -1, 0],
    [0, 0, 0, 0, 0, 0, -1, 0],
    [0, -1, 1, -1, -1, -1, -1, 0],
    [0, 1, -1, -1, -1, -1, -1, 0],
    [-2, 1, -2, -1, 1, -1, -2, 0],
    [0, -2, -1, -1, 1, -2, 1, -2],
    [-1, -1, -1, 0, 0, 1, -1, -1],
    [-3, 1, -1, -2, 2, -1, -3, -2],
    [-1, -1, 0, -1, 1, -1, -1, 0],
    [-1, -2, 1, 1, -2, -1, -1, -1],
    [-1, 1, -1, 0, 0, -1, -1, -1],
    [-3, 2, -2, -1, 1, -2, -3, -1],
    [0, 1, -1, 0, -1, -1, -1, -1],
    [0, -1, -1, -1, 0, -1, 1, -1],
    [-1, 0, 0, -1, 1, -1, -1, -1],
    [0, -2, -2, -1, 1, 1, -2, -2],
    [0, 0, 0, 0, 0, 0, 0, -1],
    [-2, -1, 1, -1, -1, -1, 1, -1],
    [0, -1, -1, -1, 1, -1, 0, -1],
    [-2, -1, -1, 1, -1, 1, -2, -1],
    [-1, 2, -3, 1, -1, -2, -3, -3],
    [0, -1, 1, 0, -1, -1, -1, -1],
    [0, -1, -1, -1, 1, 0, -1, -1],
    [-1, -3, 2, 1, -3, -2, -1, -2],
    [-1, -1, 0, 1, -1, 0, -1, -1],
    [0, -1, -1, -1, 0, 1, -1, -1],
    [-1, 0, -1, 1, -1, 0, -1, -1],
    [-1, -2, -3, -1, 1, 2, -3, -2],
    [-1, -1, 0, -1, -1, -1, 1, 0],
    [1, -1, 1, -2, -1, -1, -2, 1],
    [-1, -1, -1, -1, 0, 1, 0, 1],
    [0, -1, -1, 0, 1, -1, 1, -1],
    [1, -3, -3, -1, 2, -4, 2, -3],
    [-1, -2, 2, -1, 0, -2, -2, 1],
    [-1, 1, -1, -1, -1, -1, 0, 1],
    [1, -2, -1, 0, 2, 1, -2, -2],
    [1, -1, -1, 0, 1, 0, -1, -1],
    [-1, -1, 1, 0, 0, -1, -1, 1],
    [-1, 0, 0, 1, 1, 1, 1, 0],
    [0, -1, -1, 0, -1, 1, -1, 1],
    [-1, -2, 1, -2, -1, 1, 2, 1],
    [1, 0, 0, -1, -1, -1, -1, 1],
    [-1, -1, 0, -1, 1, 2, 2, 2],
    [1, 0, -1, 0, -1, 1, 1, -1],
    [-1, 1, 1, -1, 0, 0, 1, -1],
    [-2, 0, 0, 1, 2, 1, 2, 1],
    [-3, -1, -1, 3, 5, 2, 5, 2],
    [1, 0, 0, 0, 0, 0, 0, 0],
    [1, -2, -2, -2, -1, 2, -1, -1],
    [1, 1, -2, 1, 1, 1, 1, 1],
    [1, -1, -1, 0, -1, 0, 1, -1],
    [1, -1, -1, 0, -1, 0, -1, 1],
    [-1, 1, 1, 1, 0, 1, 0, 1],
    [1, 1, 1, 1, 1, 1, 1, -2],
    [1, 1, 1, 0, -1, 0, 0, 1],
    [1, 2, -1, -1, -2, -2, -1, -1],
    [0, 0, 0, 1, 0, 0, 0, 0],
    [1, -1, -1, 0, 1, -1, 0, -1],
    [-1, -2, -2, 1, 1, 2, -1, -2],
    [-1, -1, 0, 1, 2, 1, 2, 1],
    [2, 2, 2, 1, -1, 0, -1, 0],
    [2, 0, 1, 2, -1, 1, 1, -1],
    [1, 1, 0, 0, -1, -1, -1, -1],
    [1, 1, 1, 1, 0, 0, -1, 0],
    [1, 1, 0, 1, 0, -1, -1, -1],
    [1, 0, 1, 0, -1, -1, -1, -1],
    [1, 2, 2, 1, -2, 0, 0, 1],
    [-1, 0, -1, -1, 2, 2, 1, 2],
    [0, -1, 0, -1, 1, -1, 1, -1],
    [2, 0, 1, -2, -1, -1, -2, 1],
    [1, -2, -1, -1, -2, 1, -2, 1],
    [2, 1, 1, 1, -1, -1, -1, -1],
    [-1, 1, 1, 1, 1, 0, 1, 0],
    [1, 1, -1, 1, -1, 2, 2, -1],
    [2, 1, 1, 2, -2, 1, 1, 0],
    [-3, 1, 1, 2, 2, 1, 2, 1],
    [1, 1, 1, 0, 0, 0, -1, 1],
    [2, 2, 1, 2, 1, -1, -1, 0],
    [1, 1, 0, 1, 1, -1, -1, 0],
    [1, 1, 1, -1, 2, -1, 2, -1],
    [1, -1, -1, 0, 0, -1, 1, -1],
    [0, -2, 2, 1, -2, -1, -1, -2],
    [4, 4, 1, 3, 3, -2, -2, 1],
    [-1, 1, -1, 0, -1, -1, 1, 1],
    [-1, 0, -1, -1, -1, -1, 1, 1],
    [-1, 0, -1, -1, 1, 1, 0, 1],
    [0, 0, -1, 0, 1, 1, 1, 1],
    [1, 1, -1, 2, 1, -2, -2, -1],
    [0, -1, 0, 0, 1, 1, 1, 1],
    [3, 2, 3, 2, -1, 0, -1, -1],
    [1, 1, 1, 1, -1, 0, 0, 0],
    [-1, 0, 1, -1, -1, 0, 1, -1],
    [3, 3, 2, 3, 0, -1, -1, -1],
    [2, 1, 2, 1, -1, 0, -1, -1],
    [-1, 1, 1, -1, 0, 1, 0, -1],
    [0, 0, 0, 0, 0, 0, 0, 1],
    [2, 2, 1, 1, 1, 0, -2, 1],
    [3, -1, 2, 2, 2, 3, -1, -1],
    [2, 2, 1, 1, 1, -2, 0, 1],
    [2, -1, 1, 1, 1, 2, -1, -1],
    [-2, -1, -1, 3, 4, 1, 4, 2],
    [0, 1, -1, 1, 0, 1, 1, -1],
    [2, 1, 2, 2, -1, 0, 0, -1],
    [1, 1, 0, 0, -1, 1, 1, 1],
    [1, 1, 1, 0, -1, 0, -1, 0],
    [0, 1, -2, 1, 2, 1, 2, 1],
    [1, 2, 2, 1, 0, -2, 1, 1],
    [-2, -1, -1, 2, 2, 1, 1, -1],
    [1, 1, -1, -1, 0, 0, 1, -1],
    [1, -1, -1, 0, 2, 1, 2, 2],
    [-1, -1, 1, 1, 0, -1, 0, 1],
    [1, 0, -1, -1, 0, -1, 1, -1],
    [-1, 1, -1, 2, 1, -1, 1, 2],
    [1, 0, 1, 1, 1, 1, -1, 0],
    [-1, -3, -3, -1, -2, 3, -2, 1],
    [1, 2, -1, 1, 0, 2, 2, -1],
    [2, 1, 1, 0, -2, -1, -2, -1],
    [1, 1, 0, -1, -1, -1, -1, 0],
    [1, 1, 0, 1, -1, 1, 1, 0],
    [1, 2, -3, 1, 2, 2, 2, 1],
    [0, 1, 1, 0, 1, 0, 1, -1],
    [2, 3, 2, 1, 1, -3, 1, 1],
    [-2, 1, -2, -1, -2, -2, 1, 2],
    [1, 1, 1, 1, 1, 1, -2, 1],
    [1, 0, 1, -1, -1, -1, -1, 0],
    [1, 1, 0, 1, 1, 0, -1, 1],
    [1, 1, 1, 0, 0, -1, 0, 1],
    [0, 1, -1, 1, 1, -1, 0, 1],
    [-1, 1, 1, 0, 1, 1, 1, 0],
    [2, -1, 1, -3, -2, -1, -3, 2],
    [3, 2, 2, -1, -1, -1, -1, 2],
    [1, 2, 2, 1, 1, -2, 1, 0],
    [-1, -1, -1, 2, 2, 0, 2, 1],
    [1, -1, 0, -1, -1, 0, -1, 1],
    [-1, 0, -1, 1, 2, 1, 2, 1],
    [2, 1, 1, -1, -2, -1, -2, 0],
    [1, -1, 0, 0, 1, 1, -1, -1],
    [2, 4, 3, 2, 1, -4, 2, 1],
    [3, 1, 1, -2, -2, -2, -2, 1],
    [4, 1, 2, -3, -3, -2, -3, 1],
    [1, 2, 2, 1, -1, -1, 0, 1],
    [1, -1, -1, -1, -1, 1, 0, -1],
    [2, 1, 2, -1, -1, 0, -1, 1],
    [2, 1, 1, 2, 0, 1, 1, -2],
    [2, 1, 1, 0, -2, 1, 1, 2],
    [0, 1, 0, 1, 1, 1, 1, -1],
    [-1, -1, 1, -1, -1, 0, 1, 0],
    [0, -1, 1, -1, 0, -1, -1, 1],
    [-2, 2, 2, -2, 1, 1, 0, -1],
    [-2, 3, 3, -2, 1, 1, 1, -2],
    [-1, 1, -1, 1, -1, 1, 0, -1],
    [1, 1, -2, 0, 1, 2, 1, 1],
    [0, -1, 1, -1, 0, 1, 1, 1],
    [1, -1, -1, 1, -1, -1, 0, 1],
    [1, 1, 0, -1, 1, 0, 1, -1],
    [0, -1, 1, 1, -1, 0, -1, -1],
    [2, 2, 2, 0, -1, 0, -1, 1],
    [3, 1, -3, -3, -1, -1, 2, -3],
    [1, 1, 0, 0, 1, 1, 1, -1],
    [1, -1, -1, 1, 0, -1, 1, 0],
    [3, 1, 2, 1, -2, -1, -2, -2],
    [-1, 1, 0, 1, 0, -1, 1, 1],
    [2, 3, 3, 1, -1, -1, -1, 1],
    [2, 2, 0, 2, 2, -1, -1, 1],
    [-1, 1, 1, 2, -1, -1, 1, 2],
    [1, 0, 1, 1, -1, 0, 0, -1],
    [0, 2, -1, 2, 1, 2, 2, -1],
    [3, 4, 4, 1, -2, 0, -2, 1],
    [1, -1, -2, -1, 0, -2, 2, -2],
    [2, 3, 3, 1, -1, 0, -2, 1],
    [1, 0, 0, 1, -1, 1, 1, -1],
    [1, 2, 2, 0, -1, 1, -1, 1],
    [0, 1, -1, 1, 1, 1, 1, 0],
    [2, 3, -1, 3, 3, -1, -1, 2],
    [1, 1, -1, 0, 1, 1, 0, 1],
    [1, 1, -1, 1, 1, 0, 0, 1],
    [-2, -1, -1, -2, -2, -1, 2, 1],
    [1, -3, -2, -1, 2, 1, -3, -3],
    [1, 1, 0, 0, 1, 1, -1, 1],
    [0, 1, 0, 0, 0, 0, 0, 0],
    [-2, -1, 2, 1, -1, -2, 0, 1],
    [1, -2, 1, 0, 1, 2, 1, 2],
    [-1, 2, -2, -2, -1, -2, -1, 1],
    [1, 2, -1, 2, 2, -1, 0, 2],
    [1, 0, 1, 1, 0, 1, 0, -1],
    [1, -1, 2, -1, -1, 2, 1, 2],
    [0, 1, -1, -1, 1, 1, 0, 1],
    [1, -1, -1, 0, 1, -1, 1, 0],
    [1, 1, 2, -1, -1, 1, -1, 1],
    [0, 2, 1, 2, 1, -2, 2, 1],
    [1, -1, -2, -1, -1, -1, 2, -2],
    [2, -1, -2, 1, 2, -1, -1, -2],
    [3, -1, -3, -1, -3, 1, 2, -3],
    [1, 2, 1, 1, 2, 2, 2, -3],
    [0, -1, -1, 0, 1, 1, -1, -1],
    [0, 1, 0, 1, 1, -1, 1, 1],
    [0, 1, 1, 1, 2, 1, 2, -2],
    [-1, 0, 0, 0, 1, 1, 1, 1],
    [-1, -1, 0, -1, -1, 0, 1, 1],
    [-1, 0, 1, 1, 0, -1, 1, 1],
    [-1, 1, 1, -1, 1, 0, 0, -1],
    [2, -2, -1, 1, -1, -1, 2, -1],
    [-1, -2, 1, -1, 1, -2, -2, 1],
    [-1, 1, 1, 2, 0, -1, 2, 2],
    [-1, 1, 1, -1, 1, 1, 0, 0],
    [1, -2, 1, 1, 1, 2, 1, 1],
    [0, 0, 1, 0, 0, 0, 0, 0],
    [-1, 2, 2, -1, 1, 0, 1, -2],
    [0, -1, -1, 1, 1, -1, 1, 0],
    [1, 0, 1, -1, -1, 1, 0, 1],
    [1, 0, 0, -1, 1, -1, 1, -1],
    [-1, 0, 1, 1, -1, -1, 0, 1],
    [1, -1, 0, 1, -1, 0, 1, -1],
    [-2, 2, 1, -2, 1, 2, -1, 0],
    [1, 2, 3, -1, -2, 2, -2, 1],
    [1, -1, -1, -1, 1, -2, 1, -1],
    [-4, 1, 1, 2, 3, 2, 3, 1],
    [1, -1, 1, 2, -2, -1, 1, -2],
    [0, 1, 1, 1, 1, -1, 1, 0],
    [0, 1, -1, 2, 2, -1, 1, 2],
    [-1, 1, 0, -1, 1, 1, -1, 0],
    [2, -1, 2, 1, 2, 2, -1, 0],
    [-3, 1, 1, 1, 2, 2, 2, 1],
    [1, -1, 1, 1, 1, 1, 0, 0],
    [-1, 0, 2, -2, 1, -2, 1, -2],
    [0, -1, 1, 1, 1, 1, 1, 0],
    [-1, 1, 1, 1, 0, 0, 1, 1],
    [0, 0, -1, 1, 1, 0, 1, 1],
    [-1, 0, 1, -1, 1, -1, 0, -1],
    [-1, 0, 0, 1, 1, 0, 1, 1],
    [1, -1, -1, 0, 1, 0, 1, 1],
    [-1, 0, 1, -1, 0, -1, 1, -1],
    [1, -1, 1, 0, 0, 1, 1, 1],
    [-1, 1, 0, 2, 1, -1, 2, 2],
    [1, 1, 0, 1, 1, -1, 0, 1],
    [0, 0, 0, 0, 1, 0, 0, 0],
    [1, -1, 1, 1, 0, 1, 1, 0],
    [4, 3, 3, 3, -1, -1, -1, -1],
    [0, 0, 0, 0, 0, 0, 1, 0],
    [1, -2, -2, -1, 1, -3, 2, -2],
    [-1, 1, -1, 0, 1, -1, -1, 0],
    [-1, -1, -1, -1, 2, 3, 2, 3],
    [-1, 1, 0, -1, 1, 0, -1, -1],
    [0, 0, 1, -1, 1, -1, 1, -1],
    [1, 0, -1, -1, -1, 0, 1, -1],
    [0, 1, -1, 1, 0, -1, -1, -1],
    [0, 1, 1, 0, 0, 1, -1, 1],
    [0, 0, 0, 0, 0, 1, 0, 0],
    [-1, 1, 1, 0, 1, 1, 0, 1],
    [-2, 1, 1, 2, 1, 0, 2, 1],
    [2, 1, 3, -2, -2, 1, -1, 2],
    [2, -1, 2, -1, 1, 1, -1, 1],
    [1, -2, -2, 0, 1, -2, 1, -1],
    [-1, 2, 2, -1, 1, 1, 1, -1],
    [-1, 1, -1, -2, 2, 2, 0, 2],
    [-3, 3, 2, -3, 1, 2, -1, -1],
    [1, 0, 1, 0, -1, 1, 1, 1],
    [1, 0, 1, -1, 0, 0, -1, 1],
    [1, 1, 1, 0, 1, -1, 1, 0],
    [-2, 1, 1, 1, 1, 1, 1, 1],
    [0, 1, 0, -1, 1, 1, -1, 1],
    [1, -1, 1, -1, 0, 1, 0, 1],
    [2, 2, -1, 2, -1, 3, 3, -1],
    [1, -2, -2, -1, 2, -3, 1, -2],
    [0, 1, 1, 1, -1, 0, 0, 1],
    [1, -1, -2, 1, 1, -1, 2, 1],
    [1, 1, 1, 1, -2, 1, 1, 1],
    [3, -1, 2, -3, -1, 2, -1, 3],
    [-1, 2, 0, -2, 2, 2, -1, 1],
    [-1, 1, -1, 3, 2, -1, 2, 3],
    [-1, 1, 0, -1, 1, 1, 0, 1],
    [1, -2, 2, -1, 0, 2, 1, 2],
    [1, 1, 1, 1, 0, 0, 0, -1],
    [-1, 1, 1, -1, 0, 1, -1, 0],
    [1, 1, 1, 1, 0, -1, 0, 0],
    [1, -1, 1, 0, 1, 1, 0, 1],
    [-2, 3, 1, -2, 2, 3, -1, 1],
    [1, -2, 2, 2, 1, 2, 1, 0],
    [1, 0, 1, 1, -1, 1, 1, 0],
    [-1, -1, -1, 1, 1, 0, 1, 0],
    [-1, 2, -1, -3, 3, 2, -1, 2],
    [1, -1, 1, 0, 1, 1, -1, 0],
    [-2, 2, 1, 4, 1, -2, 4, 3],
    [2, -1, 2, 0, 2, 2, -1, 1],
    [0, 0, 1, 1, 1, 1, 1, -1],
    [1, 1, -1, 0, 0, 1, 1, 0],
    [2, 2, 1, 1, 2, 1, -3, 2],
    [0, 1, -1, 1, 1, -1, -1, 0],
    [0, -1, 1, 1, -1, -1, 0, -1],
    [-2, 2, 1, -1, 2, 2, 0, 1],
    [1, 1, 1, 0, 1, 1, -2, 2],
    [-2, 1, 1, 3, 1, -1, 3, 2],
    [-1, 1, 2, -1, 1, -1, 1, -2],
    [0, 1, 1, -1, -1, 1, -1, 0],
    [1, -2, -1, -1, 1, 1, -2, -2],
    [1, -2, -2, 1, 1, -2, 2, 0],
    [-2, 1, 1, 0, 2, 2, 1, 1],
    [-1, -1, 0, -1, 0, 1, 1, 1],
    [-1, -2, -2, -1, 0, 2, -2, -1],
    [-1, -1, 0, -1, 0, -1, 1, -1],
    [-1, -1, 0, -1, 1, -1, 0, -1],
    [-2, -1, 0, -2, 2, -2, -1, -1],
    [-1, -1, -1, -2, 2, -1, -1, -1],
    [-2, -1, -1, -3, 3, -2, -2, -1],
    [-2, -1, 1, -2, 0, -2, 1, -2],
    [-2, -1, 1, -2, 1, -2, 0, -2],
    [-3, -1, 1, -3, 2, -3, -1, -2],
    [-3, -2, 2, -4, 1, -4, 1, -4],
    [-1, -1, 1, -1, 0, -1, 0, -1],
    [-1, -2, 0, -2, 1, -2, 1, -2],
]

ddt = [
    [sum([sbox[x] ^ sbox[x ^ i] == j for x in range(16)]) for j in range(16)]
    for i in range(16)
]
# for item in ddt:
#     print(item)

points = []
for i in range(16):
    for j in range(16):
        if ddt[i][j] != 0:
            points.append(
                list(map(int, list(format(i, "04b"))))
                + list(map(int, list(format(j, "04b"))))
            )


def evaluate(point, ineq):
    point.append(1)
    if sum([point[i] + ineq[i] for i in range(9)]) >= 0:
        return True
    else:
        return False


cpoints = []
for i in range(256):
    tmp = map(int, list(format(i, "08b")))
    if tmp not in points:
        cpoints.append(tmp)

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
    res.append([ineqs[index]])
    ineqs.pop(index)
    new_cpoints = []
    for p in cpoints:
        if evaluate(p, res[-1]):
            new_cpoints.append(p)
    cpoints = new_cpoints
