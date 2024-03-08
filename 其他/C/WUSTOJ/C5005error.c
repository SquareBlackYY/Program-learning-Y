#include <stdio.h>
int main()
{
    int n, i, k, cnt, m;
    double a;
    scanf("%d", &n);
    for (i = 1, k = 2, a = 0, cnt = 0; cnt < n; cnt++)
    {
        a += 1.0 * k / i;
        m = k;
        k += i;
        i = m;
    }
    printf("%lf", a);
    return 0;
}