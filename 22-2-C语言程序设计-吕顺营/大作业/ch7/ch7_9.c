// ch7_9.c

#include <stdio.h>

int Age(int n)
{
    if (n == 1)
        return 10;
    else
        return Age(n - 1) + 2;
}

void ch7_9()
{
    printf("第5个人的年龄是：%d\n", Age(5));
}