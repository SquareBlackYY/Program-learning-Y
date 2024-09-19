// ch6_17.c

#include <stdio.h>

void ch6_17()
{
    int x, y, z, count = 0;
    
    for (x = 1; x <= 20; x++)
        for (y = 1; y <= 20; y++)
            for (z = 1; z <= 50; z++)
                if ((x * 10 + y * 5 + z) == 100 && (x + y + z) == 50)
                    printf("方案%d：10元纸币%d张，5元纸币%d张，1元纸币%d张\n", ++count, x, y, z);

    printf("共有%d种兑换方案\n", count);
}