// ch6_4.c

#include <stdio.h>

void ch6_4()
{
    int n, i;
    float sum_square = 0, sum_cubic = 0;

    printf("请输入n的值：");
    scanf("%d", &n);

    for (i = 1; i <= n; i++)
    {
        sum_square += (float)i * i;
        sum_cubic += (float)i * i * i;
    }

    printf("1到%d之间的所有数的平方和为%.2f，立方和为%.2f", n, sum_square, sum_cubic);
}
