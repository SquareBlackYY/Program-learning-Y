// ch5_2.c

#include <stdio.h>

void ch5_2()
{
    int num;

    printf("输入一个整数：");
    scanf("%d", &num);

    if (num % 2 == 0)
        printf("%d是一个偶数\n", num);
    else
        printf("%d是一个奇数\n", num);
}
