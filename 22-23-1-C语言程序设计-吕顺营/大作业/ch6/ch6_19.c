// ch6_19.c

#include <stdio.h>
#include <math.h>

void ch6_19()
{
    int i, day;
    double rich = 0, stranger = 0;

    for (i = 0, day = 1; day <= 30; i++, day++)
    {
        stranger += pow(2, i) / 100;
        rich += 100000;
    }

    printf("陌生人总计支付了%.2lf元，百万富翁总共支付了%.2lf元\n", stranger, rich);
}