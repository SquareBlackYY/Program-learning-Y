// ch6_13.c

#include <stdio.h>

void ch6_13()
{
    int number, sum = 0;

    printf("请逐个输入整数，输入0表示结束：");
    scanf("%d", &number);

    while (number != 0)
    {
        if (number > 0)
            sum += number;
        scanf("%d", &number);
    }

    printf("所有正数的和为%d\n", sum);
}
