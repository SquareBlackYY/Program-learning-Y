def fibonacci(n):
    if n == 1 or n == 2:
        return 1
    return fibonacci(n - 1) + fibonacci(n - 2)

i = 1
result = fibonacci(i)
while result <= 1000:
    print(result, end = ' ')
    i += 1
    result = fibonacci(i)
