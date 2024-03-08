// ch7_10.c

#include <stdio.h>

void ch7_10()
{
    int sum, a, b, c;

    printf("请输入5个数的和值：");
    scanf("%d", &sum);

    for (a = 1; a <= 9; a++)
        for (b = 0; b <= 9; b++)
            for (c = 0; c <= 9; c++)
                if ((a + b) * 2 + c * 2 == sum - 27)
                {
                    printf("观众心里想的数是：%d%d%d\n", a, b, c);
                    return;
                }

    printf("你算错了！\n");
}