#include <stdio.h>
void main()
{
    int year, month, daysofmonth, i, week, day[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    printf("����һ����ݣ�");
    scanf("%d", &year);
     if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
         day[1]++;
    printf("=====%d�� ����=====\n", year);
    week = (((year - 1) % 100) + ((year - 1) % 100) / 4 + ((year - 1) / 100) / 4 - 2 * ((year - 1) / 100) + 36) % 7;
    for (month = 1, i = 0; month <= 12; month++)
    {
        printf("--------%2d ��--------\n", month);
        printf(" �� һ �� �� �� �� ��\n", month);
        for (daysofmonth = 1 - (i % 7); daysofmonth <= day[month - 1]; daysofmonth++)
        {
            for(;week>0;week--)
            {
                printf("   ");
                i++;
            }
            if (daysofmonth >= 1 && daysofmonth <= day[month - 1])
            {
                printf(" %2d", daysofmonth);
                i++;
            }
            else
                printf("   ");
            if (i % 7 == 0 || daysofmonth == day[month - 1])
                printf("\n");
        }
    }
}