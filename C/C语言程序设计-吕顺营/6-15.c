#include <stdio.h>
int main()
{
    int chick;
    for(chick = 1;chick <= 98;chick++)
        if(chick * 2 + (98 - chick) * 4 == 386)
            printf("鸡有%d只,兔有%d只\n", chick, 98 - chick);
}