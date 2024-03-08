#include <stdio.h>
int main()
{
    int n, min, i, a[100], sign0, mid;
    while (scanf("%d", &n) != EOF)
    {
        if (n == 0)
            break;
        for (i = 0, sign0 = 0; i < n; i++)
        {
            scanf("%d", &a[i]);
            if (i == 0)
                min = a[0];
            if (a[i] < min)
            {
                min = a[i];
                sign0 = i;
            }
        }
        mid = a[0];
        a[0] = min;
        a[sign0] = mid;
        for (i = 0; i < n; i++)
        {
            if (i != 0)
                printf(" ");
            printf("%d", a[i]);
        }
        printf("\n");
    }
    return 0;
}