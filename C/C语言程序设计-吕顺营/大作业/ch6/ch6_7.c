// ch6_7.c

#include <stdio.h>

void ch6_7()
{
    float growth_rate;
    int years = 0;

    printf("请输入产值增长率：");
    scanf("%f", &growth_rate);

    while (1)
    {
        if (100 * (1 + growth_rate / 100) <= 200)
        {
            years++;
            continue;
        }
        break;
    }

    printf("产值在%d年后可翻一倍。\n", years);
}