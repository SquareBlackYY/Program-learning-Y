// ch8_12.c

#include <stdio.h>

int fibonacci(int n)
{
    if (n == 0 || n == 1)
        return 1;
    else
        return fibonacci(n - 1) + fibonacci(n - 2);
}

void ch8_12()
{
    int pairs = fibonacci(12);
    printf("一年之后, 一共有%d对兔子\n", pairs);
}
