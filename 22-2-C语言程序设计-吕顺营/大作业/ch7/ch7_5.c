// ch7_5.c

#include <stdio.h>

int fact(int n)
{
    if (n == 1) // 递归基
        return 1;
    else
        return n * fact(n - 1); // 递归调用
}

void ch7_5()
{
    int n, i;

    printf("请输入一个正整数：");
    scanf("%d", &n);

    for (i = 1; i <= n; i++)
        printf("%d! = %d\n", i, fact(i));
}