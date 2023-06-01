// ch9_5.c

#include <stdio.h>
#include <stdlib.h>

int IsLeapYear(int year) // 判断闰年
{
    return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
}

int DayofYear(int year, int month, int day)
{
    int i, days = 0;

    int MonthDays[2][12] = {
        {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
        {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}}; // 平闰年每月天数

    for (i = 1; i < month; i++)
        days += MonthDays[IsLeapYear(year)][i - 1];

    days += day;
    return days;
}

void MonthDay(int year, int yearDay, int *pMonth, int *pDay)
{
    int i, days = 0;

    int MonthDays[2][12] = {
        {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
        {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}}; // 平闰年每月天数

    for (i = 1; i <= 12; i++)
    {
        if (yearDay <= MonthDays[IsLeapYear(year)][i - 1])
            break;
        yearDay -= MonthDays[IsLeapYear(year)][i - 1];
    }

    *pMonth = i;
    *pDay = yearDay;
}

void ch9_5()
{
    int year, month, day, yearDay, pMonth, pDay;
    int choice;

    do
    {
        printf("1.year/month/day->yearDay\n");
        printf("2.yearDay->year/month/day\n");
        printf("3.Exit\n");
        printf("Please enter your choice:");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("请输入年月日：");
            scanf("%d %d %d", &year, &month, &day);

            yearDay = DayofYear(year, month, day);
            printf("%04d年%02d月%02d日是这一年的第%d天\n", year, month, day, yearDay);
            break;
        case 2:
            printf("请输入年/天数：");
            scanf("%d %d", &year, &yearDay);

            MonthDay(year, yearDay, &pMonth, &pDay);
            printf("%d年的第%d天是%d月%d日\n", year, yearDay, pMonth, pDay);
            break;
        case 3:
            break;
        default:
            printf("Invalid choice, please enter again.\n");
        }
    } while (choice != 3);
}