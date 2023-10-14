up_factor = 0.001
down_factor = 0.001
result = 1
for i in range(365):
    if (i + 1) % 7 == 0:
        result *= 1 - down_factor
    else:
        result *= 1 + up_factor
print(f"结果为:{result}")
