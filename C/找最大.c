#include <stdio.h>
int main()
{
    int i, max, cnt = 1;
    scanf("%d", &i);
    max = i;
    for (; cnt < 5; cnt++)
    {
        scanf("%d", &i);
        if (i > max)
        {
            max = i;
        }
    }
    printf("max = %d", max);
    return 0;
}