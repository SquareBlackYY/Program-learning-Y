// ch6_18.c

#include <stdio.h>

void ch6_18()
{
    int a, b;

    for (a = 1; a <= 9; a++)
        printf("%3d", a);
    printf("\n");

    for (a = 1; a <= 9; a++)
        printf("  -");
    printf("\n");

    for (b = 1; b <= 9; b++)
    {
        for (a = 1; a <= 9; a++)
            if (a < b)
                printf("   ");
            else
                printf("%3d", a * b);
        printf("\n");
    }
}