// ch6_5.c

#include <stdio.h>

void ch6_5()
{
    int f, c;

    printf("华氏温度与摄氏温度的对照表：\n");
    printf("℉\t℃\n");

    for (f = 0; f <= 300; f += 10)
    {
        c = 5 * (f - 32) / 9;
        printf("%d\t%d\n", f, c);
    }
}