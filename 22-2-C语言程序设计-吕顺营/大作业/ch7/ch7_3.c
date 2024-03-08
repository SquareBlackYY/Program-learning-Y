// ch7_3.c

#include <stdio.h>

int max(int a, int b)
{
    return a > b ? a : b; // 三目运算符
}

void ch7_3()
{
    int a, b;

    printf("请输入两个整数：");
    scanf("%d %d", &a, &b);

    printf("最大值为%d\n", max(a, b));
}