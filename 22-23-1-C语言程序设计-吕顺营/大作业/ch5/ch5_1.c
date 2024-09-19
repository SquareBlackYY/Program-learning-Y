// ch5_1.c

#include <stdio.h>

void ch5_1()
{
    float num;

    printf("输入任意一个实数：");
    scanf("%f", &num);

    if (num < 0)
        num = -num;

    printf("该实数的绝对值为：%.2f\n", num);
}