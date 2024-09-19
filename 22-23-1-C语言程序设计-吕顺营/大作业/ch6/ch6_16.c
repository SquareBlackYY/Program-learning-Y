// ch6_16.c

#include <stdio.h>

void ch6_16()
{
    int cock, hen, chick;

    for (cock = 0; cock <= 20; cock++)
        for (hen = 0; hen <= 33; hen++)
        {
            chick = 100 - cock - hen;
            if (chick % 3 == 0 && cock * 5 + hen * 3 + chick / 3 == 100 && chick >= 0)
                printf("公鸡%d只，母鸡%d只，小鸡%d只\n", cock, hen, chick);
        }
}