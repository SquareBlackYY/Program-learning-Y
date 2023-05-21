// ch5_10.c

#include <stdio.h>

void ch5_10()
{
    int year, month, days;
    
    printf("请输入一个年份和月份：");
    scanf("%d %d", &year, &month);

    if (month < 1 || month > 12)
    {
        printf("%d是一个不合法的月份！\n", month);
        return;
    }

    switch (month)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        days = 31;
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        days = 30;
        break;
    case 2:
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
            days = 29;
        else
            days = 28;
        break;
    }

    printf("%d年%d月有%d天\n", year, month, days);
}
