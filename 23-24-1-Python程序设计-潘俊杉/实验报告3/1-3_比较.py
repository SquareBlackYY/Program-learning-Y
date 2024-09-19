dayup = pow((1.0 + 0.01), 365)
up_factor = 0.001
down_factor = 0.005
while True:
    result = 1
    for i in range(365):
        if (i + 1) % 7 == 0:
            result *= 1 - down_factor
        else:
            result *= 1 + up_factor
    if result <= dayup:
        up_factor += 0.001
    else:
        break
print("所需的进步因子为:{:.2}".format(up_factor))
