#include <stdio.h>
int main()
{
    int n, m, a[101], i, st, en, sign, k;
    while (scanf("%d %d", &n, &m) != EOF)
    {
        if (n == 0 && m == 0)
            break;
        for (i = 1, st = 1, en = n, sign = -1; i <= n; i++)
            scanf("%d", &a[i]);
        if (m < a[1])
        {
            a[0] = m;
            st = 0;
        }
        else if (m > a[n])
        {
            a[n + 1] = m;
            en = n + 1;
        }
        else
        {
            for (i = 1; i < n; i++)
            {
                if (m >= a[i] && m <= a[i + 1])
                    sign = i + 1;
            }
        }
        for (i = 0, k = 0; st <= en; st++, i++)
        {
            if (i != 0)
                printf(" ");
            if (st == sign && k != 1)
            {
                printf("%d ", m);
                k = 1;
            }
            printf("%d", a[st]);
        }
        printf("\n");
    }
    return 0;
}