// ch6_6.c

#include <stdio.h>

void ch6_6()
{
    float p, r = 0.01875, money = 0, remaining = 0;
    int i;

    printf("请输入第一年开始存款的金额：");
    scanf("%f", &p);

    remaining = money = p;

    for (i = 1; i <= 5; i++)
    {
        remaining -= 1000;
        remaining *= (1 + r * 12);
        money += 1000 + remaining;
    }

    printf("第一年开始需要存入%.2f元。\n", money);
}
