sbox = [0xC, 0x5, 0x6, 0xB, 0x9, 0x0, 0xA, 0xD, 0x3, 0xE, 0xF, 0x8, 0x4, 0x7, 0x1, 0x2]

ineqs = [
    [0, -1, 0, 0, 0, 0, 0, 0, 1],
    [-1, 0, 0, 0, 0, 0, 0, 0, 1],
    [0, 0, -1, 0, 0, 0, 0, 0, 1],
    [-1, -1, 0, -1, -1, 0, 1, 0, 3],
    [-1, 0, -1, -1, 1, 0, -1, 0, 3],
    [0, 0, 0, -1, 0, 0, 0, 0, 1],
    [0, 0, 0, 0, 0, -1, 0, 0, 1],
    [-1, -1, -1, -1, 0, 1, -1, 0, 4],
    [-2, 1, -2, -1, 1, -1, -2, 0, 6],
    [0, -1, -1, -1, -1, 1, -1, 0, 4],
    [0, -1, 1, -1, -1, -1, -1, 0, 4],
    [-1, -1, -1, -1, -1, 1, 0, 0, 4],
    [0, 0, 0, 0, 0, 0, -1, 0, 1],
    [0, 1, -1, -1, -1, -1, -1, 0, 4],
    [-1, -2, -2, -1, -1, 2, -1, 0, 6],
    [-1, -1, -1, 0, -1, 1, -1, 0, 4],
    [0, 0, 0, 0, -1, 0, 0, 0, 1],
    [-2, -2, 1, -1, -2, -1, 1, 0, 6],
    [-1, -1, -1, 1, -1, 0, -1, 0, 4],
    [-1, -1, 0, -1, 0, -1, 1, -1, 4],
    [-1, 0, -1, -1, 1, -1, 0, -1, 4],
    [-1, -3, 2, 1, -3, -2, -1, -3, 10],
    [0, 0, 0, 0, 0, 0, 0, -1, 1],
    [0, -2, -2, -2, -1, 2, -1, -1, 7],
    [0, -1, -1, -1, 0, 1, -1, -1, 4],
    [0, -1, -1, -1, 1, -2, 1, -2, 5],
    [0, -1, -1, -1, -1, 0, 1, -1, 4],
    [-1, 0, -1, 1, -1, 0, -1, -1, 4],
    [0, 1, 0, -1, -1, -1, -1, -1, 4],
    [-1, -1, 0, 1, -1, 0, -1, -1, 4],
    [0, 1, -1, 0, -1, -1, -1, -1, 4],
    [-1, 2, -3, 1, -1, -2, -3, -3, 10],
    [0, -1, 1, 0, -1, -1, -1, -1, 4],
    [0, -1, -1, -1, -1, 1, 0, -1, 4],
    [-1, 1, -1, -1, 0, -1, -1, 0, 4],
    [-2, -1, 1, -1, -1, -1, 1, -1, 5],
    [0, -1, -1, -1, 1, -1, 0, -1, 4],
    [0, -1, -1, -1, 1, 0, -1, -1, 4],
    [0, 1, 1, -2, -2, -1, -2, -2, 7],
    [0, 2, -1, -1, -2, -2, -2, -1, 7],
    [-1, -1, 1, -1, -1, -1, 0, 0, 4],
    [-1, 0, 1, -1, -1, -1, 0, -1, 4],
    [0, 0, 1, -1, -1, -1, -1, -1, 4],
    [0, -1, -1, -1, 0, -1, 1, -1, 4],
    [0, -1, 2, -1, -2, -2, -2, -1, 7],
    [-2, 1, -1, -1, 1, -1, -1, -1, 5],
    [-1, 1, 0, -1, 0, -1, -1, -1, 4],
    [-3, -2, 2, -1, -2, -2, 1, -1, 8],
    [-3, 2, -2, -1, 1, -2, -2, -1, 8],
    [-1, 1, -1, 0, 0, -1, -1, -1, 4],
    [-1, -1, 1, 0, -1, -1, 0, -1, 4],
    [1, -1, 1, 2, -2, -2, 1, -2, 5],
    [-1, -2, 1, -1, 1, -2, -2, 2, 6],
    [-1, 1, 0, 2, 1, -1, 1, 2, 0],
    [0, 0, 0, 0, 0, 1, 0, 0, 0],
    [1, 0, 1, 0, 1, 1, -1, 1, 0],
    [0, 0, 0, 0, 1, 0, 0, 0, 0],
    [-2, 2, 1, 4, 2, -2, 1, 3, 0],
    [0, 1, 1, -1, -1, 1, -1, 0, 2],
    [0, 1, -1, -1, -1, -1, 0, 1, 3],
    [-2, 0, 2, -2, -1, -1, 1, -2, 6],
    [0, 0, 0, 0, 0, 0, 0, 1, 0],
    [3, -1, 3, -1, 3, 2, -1, 2, 0],
    [0, 0, 0, 0, 0, 0, 1, 0, 0],
    [-1, 1, -2, -1, -2, -2, 1, 2, 6],
    [0, 1, 1, 0, -1, 1, -1, 1, 1],
    [-1, 1, -1, 0, 1, -1, -1, 0, 3],
    [1, -1, 1, -1, 1, 0, -1, 0, 2],
    [-1, -1, 1, 0, -1, -1, 1, 0, 3],
    [0, -1, -1, 1, -1, 0, -1, 1, 3],
    [1, 0, 1, -1, 1, 1, 0, 1, 0],
    [-1, 0, -1, -1, 3, 3, 2, 3, 0],
    [0, 0, -1, 1, 1, 1, 1, 0, 0],
    [-1, -1, 0, -1, 1, -1, -1, 1, 4],
    [3, 3, 2, -1, -1, -1, 0, 3, 0],
    [2, 3, -2, -4, -4, -4, -1, 1, 11],
    [1, 0, 0, -1, -1, -1, -1, 1, 3],
    [1, 2, -1, -2, -2, -2, 0, 1, 5],
    [2, 1, 0, -2, -2, -2, -1, 1, 5],
    [1, -2, -2, 2, 1, 0, 1, -1, 3],
    [1, 0, 1, 1, -1, -1, 0, -1, 2],
    [3, -2, -3, 1, 3, -1, -1, -3, 7],
    [2, 1, 1, 0, -2, -1, -2, -1, 4],
    [-1, -1, -1, -1, 0, 1, 0, 1, 3],
    [3, 3, 2, 3, 0, -1, -1, -1, 0],
    [-1, 0, -1, -1, -1, -1, 1, 1, 4],
    [1, 1, 1, 1, 1, 1, -2, 1, 0],
    [1, -1, -1, 0, 1, -1, 1, 0, 2],
    [1, 4, -1, -2, -4, -4, -3, -2, 12],
    [1, 0, 1, -2, 1, 2, 1, 2, 0],
    [1, 1, -1, 0, -1, 1, 1, 0, 1],
    [2, -1, 2, 2, 2, 3, -1, -1, 0],
    [0, -1, 0, 0, 1, 1, 1, 1, 0],
    [0, -1, -1, 1, 1, 0, 1, 0, 1],
    [0, -1, 1, -1, 0, -1, -1, 1, 3],
    [1, 1, 0, 0, 1, 1, -1, 1, 0],
    [2, 1, -2, -2, -1, -1, 1, -1, 5],
    [1, -1, -2, -1, -2, 0, 2, -2, 6],
    [2, 1, 1, 0, -2, 1, 1, 2, 0],
    [2, 2, 2, 0, -1, 0, -1, 1, 0],
    [2, 1, 2, -1, 1, 0, -1, 2, 0],
    [1, 1, 1, 0, 0, -1, 0, 1, 0],
    [-2, 1, 1, 3, 1, -1, 1, 2, 0],
    [2, 2, -1, 1, -1, 2, 2, 0, 0],
    [1, 2, 2, 1, 0, -2, 0, 1, 0],
    [2, 1, 1, 1, -1, 0, -1, -1, 1],
    [1, 2, 2, 1, -2, 0, 0, 1, 0],
    [1, 1, 0, 1, 0, -1, -1, -1, 2],
    [1, 1, 0, 1, 0, 1, 1, -1, 0],
    [1, 0, 1, 1, 1, 1, -1, 0, 0],
    [1, -1, 1, 1, 1, 2, -1, -1, 1],
    [3, -3, -2, 1, -1, -1, 3, -3, 7],
    [2, 2, 1, 2, -1, 1, 0, -1, 0],
    [1, 1, 1, 1, 1, 1, 1, -2, 0],
    [1, 1, 1, 0, -1, 0, 0, 1, 0],
    [1, 0, -1, -1, 0, -1, 1, -1, 3],
    [1, -1, -1, 0, -1, 0, -1, 1, 3],
    [1, 0, 1, 1, 0, 1, -1, -1, 1],
    [2, 1, 2, 2, 0, 1, -1, -1, 0],
    [4, 1, 3, -2, 3, 1, -2, 4, 0],
    [1, 1, 0, 1, -1, 1, 0, -1, 1],
    [-1, 1, 1, -1, 0, 0, 0, -1, 2],
    [3, 1, 1, -2, -2, -2, -2, 1, 5],
    [1, 1, 1, 1, 0, 0, -1, 0, 0],
    [1, 1, -1, -1, -1, 0, 1, 0, 2],
    [0, 1, -1, -1, 1, 1, 0, 1, 1],
    [1, 1, -1, 2, 1, -2, -2, -2, 5],
    [4, 3, 1, -2, -2, 1, 3, 4, 0],
    [-2, 1, 1, 2, 1, 0, 1, 1, 0],
    [1, -2, -2, -1, 1, -3, 2, -2, 7],
    [1, 0, -1, -1, -1, 0, 1, -1, 3],
    [1, 1, 1, 0, -1, 0, -1, 0, 1],
    [1, 1, 0, -1, 1, 1, 0, 1, 0],
    [2, -3, -1, -1, 2, 1, -3, -3, 8],
    [1, -1, 2, -2, 1, -1, -2, 1, 4],
    [3, 2, 2, 2, -1, 0, -1, -1, 0],
    [1, 1, -1, -1, -1, -1, 0, 0, 3],
    [1, -1, -1, 1, 0, 0, 1, -1, 2],
    [1, 1, -1, 0, 0, 1, 1, 1, 0],
    [3, 2, 3, 3, -1, -1, 0, -1, 0],
    [0, 2, -2, 1, -1, -1, -2, -2, 6],
    [0, 1, -1, 1, 0, 1, 1, -1, 1],
    [1, -1, 0, 1, -1, -1, 1, -1, 3],
    [1, 3, -1, -1, -3, -3, -2, -2, 9],
    [1, 0, 1, -1, -1, -1, -1, 0, 3],
    [0, -1, -1, 2, 2, 0, 2, 1, 0],
    [1, 1, -1, 1, 0, 1, 1, 0, 0],
    [2, 1, 1, -3, 1, 2, 1, 2, 0],
    [1, 2, -1, -2, -2, -1, 1, 1, 4],
    [1, -1, 1, 0, 1, 1, -1, 0, 1],
    [2, 1, 1, 0, 1, 1, -2, 2, 0],
    [1, 1, 1, 1, -1, 0, 0, 0, 0],
    [2, 3, 3, 2, 1, -4, 1, 1, 0],
    [1, 2, 2, 1, 0, 0, -2, 1, 0],
    [2, 2, 2, -1, 3, -1, 3, -1, 0],
    [0, 1, 1, -1, 1, 1, 1, 0, 0],
    [1, 1, 0, -1, -1, -1, -1, 0, 3],
    [0, 0, -1, 0, 1, 1, 1, 1, 0],
    [1, 2, 0, -1, -2, -2, -2, -1, 6],
    [1, 1, 1, 0, 0, 0, -1, 1, 0],
    [0, -1, 1, -1, 0, 1, 1, 1, 1],
    [-2, 0, 0, 1, 2, 1, 2, 1, 0],
    [3, 2, 3, -1, 0, -1, -1, 3, 0],
    [0, 1, -1, -1, 2, 2, 1, 2, 0],
    [1, -1, -1, 0, 1, 0, -1, -1, 3],
    [1, 0, 2, -1, -2, -2, -2, -1, 6],
    [2, 2, 2, 1, 1, -3, 1, 1, 0],
    [1, 1, 1, -1, 1, 0, 1, 0, 0],
    [0, 1, -1, 1, -1, 0, -1, -1, 3],
    [3, 2, 2, -1, -4, -2, -4, -1, 8],
    [3, 4, 4, 1, -2, 0, -2, 1, 0],
    [1, 0, 1, -1, 0, -1, -1, 1, 2],
    [0, 1, -1, 2, 1, 2, 2, -1, 0],
    [1, 2, 2, 0, -1, 1, -1, 1, 0],
    [1, 1, 1, -2, 1, 1, 1, 1, 0],
    [-1, 1, 1, 2, 1, -1, 0, 2, 0],
    [1, 1, 1, -1, 0, 0, 0, 1, 0],
    [4, 3, 3, -1, -1, -1, -1, 3, 0],
    [0, 0, -2, 1, 2, 1, 2, 1, 0],
    [0, 0, -1, 1, 1, 0, 1, 1, 0],
    [0, 0, 0, -1, 1, 1, 1, 1, 0],
    [2, 0, 1, -2, -1, -2, -2, 1, 5],
    [2, 2, 1, -1, -1, 0, 1, 2, 0],
    [-1, 0, 0, 0, 1, 1, 1, 1, 0],
    [1, 1, 0, -1, 0, 1, 1, 1, 0],
    [1, 1, 0, 0, -1, 1, 1, 1, 0],
    [-1, -1, 1, 0, 1, -1, -1, 1, 3],
    [2, 1, 1, -2, 0, 1, 1, 2, 0],
    [1, -1, -2, -2, 1, -3, 2, -2, 7],
    [1, -2, 1, 0, 1, 2, 1, 2, 0],
    [0, -1, 1, 1, -1, -1, 0, -1, 3],
    [1, 1, -1, 0, 1, 1, 0, 1, 0],
    [0, -2, -2, 3, 4, 1, 4, 1, 0],
    [1, 0, 1, -1, 0, 1, 1, 1, 0],
    [2, 1, 1, -1, -2, -1, -2, 0, 4],
    [1, 2, -1, -1, 2, 2, 0, 2, 0],
    [0, -1, 1, 1, 0, 1, -1, -1, 2],
    [0, -2, 2, 1, -2, -1, -1, -2, 6],
    [1, -2, -2, -1, 2, -3, 1, -2, 7],
    [0, 1, -1, 1, 0, -1, -1, -1, 3],
    [2, 2, -1, 2, -1, 3, 2, -1, 0],
    [0, -2, 0, 1, 2, 1, 2, 1, 0],
    [1, 0, 1, -1, 1, 0, -1, 1, 1],
    [1, -2, -2, 0, 1, -2, 1, -1, 5],
    [1, 1, 1, 1, 0, 0, 0, -1, 0],
    [1, 0, 0, 0, 0, 0, 0, 0, 0],
    [1, 3, -2, -2, 3, 4, 1, 4, 0],
    [1, -1, -1, 0, 1, 0, 1, 1, 1],
    [1, -2, -1, -2, 2, -3, 1, -2, 7],
    [1, 1, 1, 1, 0, -1, 0, 0, 0],
    [-1, -1, 0, -1, 2, 3, 3, 3, 0],
    [-3, 1, 1, 1, 2, 2, 2, 1, 0],
    [1, 1, 0, 0, -1, -1, -1, -1, 3],
    [1, 1, 0, 1, -1, 1, 1, 0, 0],
    [2, 2, -1, 0, -1, 2, 2, 1, 0],
    [1, 2, -1, -2, -2, -2, -1, 0, 6],
    [3, 3, -1, -1, -1, 2, 3, 2, 0],
    [1, -3, -2, -2, 3, -4, 1, -3, 10],
    [2, 2, 0, -1, -1, 1, 2, 2, 0],
    [2, -2, 1, -2, 1, -1, -1, -1, 5],
    [-4, 1, 1, 2, 3, 2, 3, 1, 0],
    [1, -1, -1, 0, 2, 1, 2, 2, 0],
    [0, 2, 2, 1, -1, 1, -1, 2, 0],
    [0, 0, 0, -1, 1, -1, 1, -1, 2],
    [1, 1, 0, -1, -1, 0, 1, 1, 1],
    [0, 1, 1, 1, 0, 0, -1, 1, 0],
    [1, -2, 2, -3, -1, -3, -3, 1, 9],
    [0, -1, 0, 1, 1, 1, 1, 0, 0],
    [0, 1, 1, 1, 0, -1, 0, 1, 0],
    [1, -1, -1, 1, 2, 0, 2, 1, 0],
    [0, 1, 1, -1, -1, 0, -1, -1, 3],
    [0, 1, 0, 0, 0, 0, 0, 0, 0],
    [1, -1, -1, 0, 0, -1, 1, -1, 3],
    [1, 1, 0, -2, 1, 2, 1, 2, 0],
    [0, -1, -1, 0, -1, 1, -1, 1, 3],
    [1, 1, -2, 1, 1, 1, 1, 1, 0],
    [-1, 0, 1, -1, 0, -1, 1, -1, 3],
    [-1, 0, 0, 1, 1, 1, 1, 0, 0],
    [1, 0, -1, 0, -1, 1, 1, -1, 2],
    [0, -1, 0, 1, 1, 0, 1, 1, 0],
    [2, -1, 2, 1, 2, 2, -1, 0, 0],
    [1, -1, 1, 1, 1, 1, 0, 0, 0],
    [0, 0, 0, 1, 1, -1, 1, 1, 0],
    [1, 1, 1, 1, -2, 1, 1, 1, 0],
    [0, 1, -1, 1, -1, 1, 0, -1, 2],
    [1, -1, 2, -1, 0, 2, 2, 2, 0],
    [1, -1, 0, 0, 1, 1, -1, -1, 2],
    [1, 0, 1, 0, -1, 1, 1, 1, 0],
    [-1, 3, 3, -1, 2, 2, 2, -1, 0],
    [1, 1, -1, 1, -1, 2, 1, -1, 1],
    [-1, 0, -1, -1, 1, 1, 0, 1, 2],
    [0, -1, 1, 1, -1, 0, -1, -1, 3],
    [0, -1, -1, 2, 2, 1, 2, 0, 0],
    [1, -1, -1, 0, 1, -1, 0, -1, 3],
    [1, 1, -1, 2, 0, 2, 2, -1, 0],
    [-2, 1, 2, 4, 2, -2, 1, 3, 0],
    [-1, 0, 1, -1, -1, 0, 1, -1, 3],
    [1, 1, -2, 0, 1, 2, 1, 2, 0],
    [1, -1, -1, -1, 1, -2, 1, -1, 4],
    [-1, 1, 1, 2, 0, -1, 1, 2, 0],
    [1, -1, 1, 0, 0, 1, 1, 1, 0],
    [1, -1, 0, 1, 1, 1, 0, -1, 1],
    [-1, 0, 1, 1, 0, -1, 1, 1, 1],
    [0, 1, 1, 2, 1, -2, 1, 1, 0],
    [1, -1, -1, 0, -1, 0, 1, -1, 3],
    [1, -1, 1, 2, 2, 2, 0, -1, 0],
    [-1, 1, 1, -1, 1, -1, 1, -2, 3],
    [1, -1, -1, 1, 1, 0, 0, -1, 2],
    [2, 2, 0, 1, -1, -1, -2, -2, 4],
    [0, 1, 1, 0, 1, 0, 1, -1, 0],
    [-2, 1, 2, 4, 1, -2, 2, 3, 0],
    [1, 0, -1, 1, 1, -1, -1, -1, 3],
    [2, -1, 2, 0, 2, 2, -1, 1, 0],
    [-1, -1, 0, -1, 0, 1, 1, 1, 2],
    [0, -1, 1, -1, 1, 2, 2, 2, 0],
    [2, 1, 1, -2, 1, 1, 0, 2, 0],
    [1, 0, 1, 0, -1, -1, -1, -1, 3],
    [1, -1, -1, 3, 2, 2, 2, -1, 0],
    [1, 0, 1, 1, 1, 1, 0, -1, 0],
    [1, -1, 1, 0, 1, 1, 0, 1, 0],
    [2, 0, 2, 1, -2, -1, -1, -2, 4],
    [-1, -1, -1, -1, 3, 4, 3, 4, 0],
    [0, 1, 1, 1, -1, 0, 0, 1, 0],
    [2, -2, 3, -4, -1, -4, -4, 1, 11],
    [1, 1, 0, -1, -1, -1, 0, 1, 2],
    [-1, 1, 1, 1, 0, 0, 0, 1, 0],
    [-1, 1, 0, -1, 1, 0, -1, -1, 3],
    [1, -1, 2, -2, -1, -2, -2, 0, 6],
    [1, -2, 1, 1, 1, 1, 1, 1, 0],
    [0, 0, 1, 0, 0, 0, 0, 0, 0],
    [-1, 1, 0, 1, 1, -1, 0, 1, 1],
    [2, 0, 2, -1, 2, 1, -1, 2, 0],
    [1, -1, 3, -1, -2, -3, -3, -2, 9],
    [1, -1, 0, -1, 1, 0, -1, -1, 3],
    [1, -1, -1, 2, 1, 1, 1, -1, 1],
    [2, -1, -3, -1, -3, 1, 2, -3, 8],
    [0, 0, 0, 1, 1, 1, 1, -1, 0],
    [1, -2, -3, -2, 1, -4, 3, -3, 10],
    [1, -1, 0, -1, 1, -1, 0, -1, 3],
    [1, -1, 4, -2, -3, -4, -4, -2, 12],
    [1, -1, 2, -2, 0, -2, -2, 1, 5],
    [1, 2, -2, -3, -3, -3, -1, 1, 9],
    [-1, 1, 1, 3, 1, -2, 1, 2, 0],
    [-1, 0, 1, 2, 1, -1, 1, 2, 0],
    [1, -2, 3, -2, 1, 4, 3, 4, 0],
    [1, -2, -1, -1, 2, 0, -2, -2, 6],
    [-1, 1, 0, -1, 1, -1, 0, -1, 3],
    [1, 0, -1, 1, 0, 1, 1, -1, 1],
    [0, -1, 1, 1, 1, 1, 0, -1, 1],
    [-1, 0, 0, 1, 1, 0, 1, 1, 0],
    [-2, 2, 0, -2, 1, -1, -1, -2, 6],
    [0, 0, 0, 1, 0, 0, 0, 0, 0],
    [1, -1, 1, -1, 0, -1, -1, 0, 3],
    [1, -1, -2, -1, 0, -2, 2, -2, 6],
    [-2, 2, 1, 4, 1, -2, 2, 3, 0],
    [1, -1, 0, 2, 2, 2, 1, -1, 0],
    [-1, 1, 0, 1, 0, -1, 1, 1, 1],
    [-1, 0, 1, 1, 1, -1, 0, 1, 1],
    [1, -2, -1, -1, 2, -2, 0, -2, 6],
    [-1, 1, -1, 0, -1, -1, 1, 1, 3],
    [0, -1, 1, 2, 2, 2, 1, -1, 0],
    [1, 0, -1, 2, 1, 2, 2, -1, 0],
    [-2, -1, -1, 2, -2, 0, -2, -1, 7],
    [-2, -1, 2, -1, -2, -2, 0, -1, 7],
    [-2, 2, -1, -1, 0, -2, -2, -1, 7],
    [-1, 1, 1, -2, -1, -1, -1, -2, 6],
    [-2, -1, -1, 1, -2, 1, -2, -1, 7],
]

ddt = [
    [sum([sbox[x] ^ sbox[x ^ i] == j for x in range(16)]) for j in range(16)]
    for i in range(16)
]
# for item in ddt:
 	# print(item)

points = []
for i in range(16):
    for j in range(16):
        if ddt[i][j] != 0:
            points.append(
                list(map(int, list(format(i, "04b")))) + list(map(int, list(format(j, "04b"))))
            )
# print(points)

def evaluate(point, ineq):
    point.append(1)
    if sum([point[i] * ineq[i] for i in range(9)]) >= 0:
        return True
    else:
        return False


cpoints = []
for i in range(256):
    tmp = list(map(int, list(format(i, "08b"))))
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
    res.append(ineqs[index])
    ineqs.pop(index)
    new_cpoints = []
    for p in cpoints:
        if evaluate(p, res[-1]):
            new_cpoints.append(p)
    cpoints = new_cpoints
print(res)

def get_u():
    s = ""
    for i in range(16):
        s += " + ".join([f"2 u_{i}_1 + u_{i}_0"])
    return s

def get_ineq(input, output, ineq):
    tmp = [" ".join(item) for item in list(zip(map(str, ineq[0:-1]), input + output))]
    tmp = " + ".join(tmp)
    tmp = tmp.replace("+ -", "- ")
    tmp += " >= " + str(-ineq[-1])
    return tmp

def get_var(input, i):
    s = []
    for j in range(4):
        s.append(f"{input}_{i}_{j}")
    return s

with open("present.lp", "w") as f:
    # 目标函数
    f.write("Minimize\n")
    f.write(get_u())
    
    x = [get_var("x", i) for i in range(16)]
    y = [get_var("y", i) for i in range(16)]
    print(x)
    # 约束条件
    f.write("Subject To\n")
    get_ineq(x, y, res)

    # 变量约束
    f.write("Binary\n")
    for item in x:
        f.write(item + "\n")
    for item in y:
        f.write(item + "\n")