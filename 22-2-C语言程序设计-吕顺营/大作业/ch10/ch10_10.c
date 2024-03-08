// ch10_10.c

#include <stdio.h>
#include <string.h>

void ch10_10()
{
    char *week[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    char day[20];
    int i;

    printf("请输入英文的星期几：");
    scanf("%s", day);

    for (i = 0; i < 7; i++)
        if (strcmp(day, week[i]) == 0)
        {
            printf("%d\n", i);
            break;
        }

    if (i == 7)
        printf("错误：未找到对应的星期几。\n");
}