def fibonacci(n):
    if n == 1 or n == 2:
        return 1
    return fibonacci(n - 1) + fibonacci(n - 2)

for i in range(1000):
    print(fibonacci(i + 1))