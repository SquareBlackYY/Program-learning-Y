// ch11_3.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ch11_3()
{
    int month;
    char *months[12] = {"January", "February", "March", "April", "May",
                        "June", "July", "August", "September",
                        "October", "November", "December"};
    printf("输入一个月份数: ");
    scanf("%d", &month);
    if (month >= 1 && month <= 12)
        printf("%s\n", *(months + month - 1));
    else
        printf("Illegal month\n");
}