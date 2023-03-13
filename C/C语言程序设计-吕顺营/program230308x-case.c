#include <stdio.h>
int dayofmonth(int month, int year);
void main()
{
    int year, month, daysofmonth, i, week;
    printf("请输入一个年份：");
    scanf("%d", &year);
    printf("=====%d年 年历=====\n", year);
    week = (((year - 1) % 100) + ((year - 1) % 100) / 4 + ((year - 1) / 100) / 4 - 2 * ((year - 1) / 100) + 36) % 7;
    for (month = 1, i = 0; month <= 12; month++)
    {
        printf("--------%2d 月--------\n", month);
        printf(" 日 一 二 三 四 五 六\n");
        for (daysofmonth = 1 - (i % 7); daysofmonth <= dayofmonth(month, year); daysofmonth++)
        {
            for(;week>0;week--)
            {
                printf("   ");
                i++;
            }
            if (daysofmonth >= 1 && daysofmonth <= dayofmonth(month, year))
            {
                printf(" %2d", daysofmonth);
                i++;
            }
            else
                printf("   ");
            if (i % 7 == 0 || daysofmonth == dayofmonth(month, year))
                printf("\n");
        }
    }
}

int dayofmonth(int month, int year)
{
    int day;
    switch(month)
    {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:day = 31;
            break;
        case 4:
        case 6:
        case 9:
        case 11:day = 30;
            break;
        case 2:
            if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
                day = 29;
            else
                day = 28;
            break;
    }
    return day;
}