// ch7_6.c

#include <stdio.h>

void ch7_6()
{
    int n, sum = 0, i;

    printf("请输入一个正整数：");
    scanf("%d", &n);

    for (i = 1; i <= n; i++)
        sum += fact(i);

    printf("1! + 2! + ... + %d! = %d\n", n, sum);
}