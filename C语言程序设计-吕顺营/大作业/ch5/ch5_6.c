// ch5_6.c

#include <stdio.h>

int ch5_6(void)
{
    int year, flag;
    printf("Enter year:");
    scanf("%d", &year);
    flag = ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) ? 1 : 0; // 使用条件表达式判断是否是闰年
    if (flag)
        printf("%d is a leap year!\n", year);       // 打印"是闰年"
    else
        printf("%d is not a leap year!\n", year);   // 打印"不是闰年"
    return 0;
}