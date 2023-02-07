#include <stdio.h>
int main()
{
    int n, m, a[100], i, cnt, cnt1, and0, rest;
    for (i = 0; i <= 99; i++)
        a[i] = (i + 1) * 2;
    while (scanf("%d %d", &n, &m) != EOF)
    {
        for (cnt1 = 0; (n - cnt1) >= m;)
        {
            for (cnt = 0, and0 = 0; cnt < m; cnt++, cnt1++)
                and0 += a[cnt1];
            if (cnt1 != m)
                printf(" ");
            printf("%d", and0 / m);
        }
        rest = n - cnt1;
        if (rest > 0)
        {
            for (and0 = 0; cnt1 < n; cnt1++)
                and0 += a[cnt1];
            printf(" %d", and0 / rest);
        }
        printf("\n");
    }
    return 0;
}