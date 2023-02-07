#include <stdio.h>
#include <stdlib.h>
int main()
{
    int i, k, cnt_0, cnt_1 = 1, cnt_2, cnt_3, cnt_4 = 2;
    scanf("%d", &i);
    cnt_3 = i - 1;
    for (cnt_2 = 0; cnt_2 < i; cnt_2++)
    {
        for (k = 0; k < cnt_3; k++)
        {
            printf(" ");
        }
        cnt_3 -= 1;
        for (cnt_0 = 0; cnt_0 < cnt_1; cnt_0++)
        {
            printf("*");
        }
        cnt_1 = cnt_4 * 2 - 1;
        printf("\n");
        cnt_4++;
    }
    return 0;
}