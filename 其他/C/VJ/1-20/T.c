#include <stdio.h>
int main()
{
    int n, i, a[100], x, y, mid, k;
    while (scanf("%d", &n) != EOF)
    {
        if (n == 0)
            break;
        for (i = 0; i < n; i++)
            scanf("%d", &a[i]);
        k = 1;
        while (k > 0)
        {
            for (i = 0, k = 0; i < (n - 1); i++)
            {
                if (a[i] < 0)
                    x = -1 * a[i];
                else
                    x = a[i];
                if (a[i + 1] < 0)
                    y = -1 * a[i + 1];
                else
                    y = a[i + 1];
                if (x < y)
                {
                    mid = a[i];
                    a[i] = a[i + 1];
                    a[i + 1] = mid;
                    k++;
                }
            }
        }
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