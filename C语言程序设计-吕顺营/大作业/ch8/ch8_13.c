// ch8_13.c

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void ch8_13()
{
    int *count = (int *)malloc(6 * sizeof(int));
    int i;
    
    srand(time(NULL));

    for (i = 0; i < 6; i++)
        *(count + i) = 0;

    for (i = 0; i < 6000; i++)
    {
        int num = rand() % 6;
        (*(count + num))++;
    }

    for (i = 0; i < 6; i++)
        printf("骰子面数为%d的出现概率为%.2f%%\n", i + 1, (*(count + i)) * 100.0 / 6000);

    free(count);
}