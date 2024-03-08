// ch6_11.c

#include <stdio.h>

void ch6_11()
{
    int n, m, sum, fac;

    printf("请输入n的值：");
    scanf("%d", &n);

    m = sum = fac = 1;
    while (sum < n)
    {
        m++;
        fac *= m;
        sum += fac;
    }

    printf("满足不等式的m的值是：%d\n", m);
}