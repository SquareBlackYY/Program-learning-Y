#include <stdio.h>
void main()
{
    int year, month;
    int day[12]= {31,28,31,30,31,30,31,31,30,31,30,31};
    printf("输入一个年份，一个月份：");
    scanf("%d%d",&year,&month);
    if((year %  4 == 0 && year % 100 != 0) || year % 400 == 0)
    day[1]++;
    printf("%d年%d月有%d天\n",year,month,day[month-1]);
}