// ch7_8.c

#include <stdio.h>

void ch7_8()
{
    int n = 6, i;

    while (1)
    {
        for (i = 1; i <= 5; i++)
        {
            if ((n - 1) % 5 == 0)
                n = n / 5 * 4;
            else
                break;
        }
        if (i == 6)
            break;
        n += 6;
    }

    printf("原来这堆椰子至少有%d个\n", n);
}
