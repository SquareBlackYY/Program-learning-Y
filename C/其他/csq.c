#include "stdio.h"
int main()
{
    int m, n;
    scanf("%d %d", &m, &n);
    for (; m < 10; m++)
    {
        for (; n < 10; n++)
        {
            printf("%d*%d=%d\n", m, n, m * n);
        }
        n = 1;
    }
    return 0;
}