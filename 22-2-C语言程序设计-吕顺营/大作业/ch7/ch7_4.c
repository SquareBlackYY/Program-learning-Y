// ch7_4.c

#include <stdio.h>

int lcm(int a, int b)
{
    int max = a > b ? a : b; // 找出最大值

    while (1)
    {
        if (max % a == 0 && max % b == 0)// 判断是否同时能被整除
            return max;
        max++; // 尝试下一个可能的最小公倍数
    }
}

void ch7_4()
{
    int a, b;

    printf("请输入两个正整数：");
    scanf("%d %d", &a, &b);

    printf("%d和%d的最小公倍数为%d\n", a, b, lcm(a, b));
}