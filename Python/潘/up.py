A = (1 + 0.01)**365
B = 1
for day in range(1,366):
    if day % 7 in range(1, 6):
        B *= 1 + up
    else:
        B *= 1 - 0.01
print(up)