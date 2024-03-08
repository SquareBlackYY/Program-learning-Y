#include <stdio.h>
int main()
{
    int n, a[56], i, and0, k;
    a[0] = 0;
    a[1] = 0;
    a[2] = 1;
    for (i = 3; i <= 55; i++)
        a[i] = a[i - 1] + a[i - 3];
    while (scanf("%d", &n) != EOF)
    {
        if (n == 0)
            break;
        for (and0 = 1, k = 0; k < n; k++)
            and0 += a[k + 1];
        printf("%d\n", and0);
    }
    return 0;
}