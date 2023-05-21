// ch6_17.c

#include <stdio.h>

void ch6_17()
{
    int i, j, k;
    int count = 0;

    for (i = 1; i < 40; i++)
        for (j = 1; j < 20; j++)
            for (k = 1; k < 10; k++)
                if (i + j + k == 50 && i + 2 * j + 10 * k == 100)
                {
                    printf("10元：%d张 5元：%d张 1元：%d张\n", k, j, i);
                    count++;
                }

    printf("共有%d种兑换方案\n", count);
}
