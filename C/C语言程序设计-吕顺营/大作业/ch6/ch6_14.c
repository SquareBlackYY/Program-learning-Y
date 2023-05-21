// ch6_14.c

#include <stdio.h>

void ch6_14()
{
    int man, woman, child, total;

    for (man = 0; man <= 30; man++)
        for (woman = 0; woman <= 30 - man; woman++)
        {
            child = 30 - man - woman;
            total = 3 * man + 2 * woman + child;
            if (total == 50)
                printf("男人有%d个，女人有%d个，小孩有%d个\n", man, woman, child);
        }
}
