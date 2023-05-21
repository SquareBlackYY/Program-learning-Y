// ch6_10.c

#include <stdio.h>
#include <math.h>

void ch6_10()
{
    int n;

    printf("三位数水仙花数有：");

    for (n = 100; n <= 999; n++)
        if (n == pow(n / 100, 3) + pow(n / 10 % 10, 3) + pow(n % 10, 3))
            printf("%4d", n);
    
    printf("\n");
}