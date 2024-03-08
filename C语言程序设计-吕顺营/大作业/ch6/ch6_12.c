// ch6_12.c

#include <stdio.h>

void ch6_12()
{
    int input, output = 0;

    printf("请输入一些正数，以负数或零结束：");

    for (;;)
    {
        scanf("%d", &input);
        if (input <= 0)
            break;
        output += input;
    }

    printf("这些正数的和为：%d\n", output);
}