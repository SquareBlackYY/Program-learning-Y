#include <stdio.h>
int main()
{
    int a[4], n, i, x, y, z;
    scanf("%d %d %d", &a[1], &a[2], &a[3]);
    for (;;)
    {
        for (n = 1; n < 3; n++)
        {
            if (a[n] > a[n + 1])
            {
                a[0] = a[n];
                a[n] = a[n + 1];
                a[n + 1] = a[0];
            }
            if (a[1] <= a[2] && a[2] <= a[3])
                break;
        }
        if (a[1] <= a[2] && a[2] <= a[3])
            break;
    }
    printf("%d",a[2]);
    return 0;
}