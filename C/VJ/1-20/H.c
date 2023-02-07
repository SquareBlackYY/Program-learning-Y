#include <stdio.h>
int main()
{
    int n, i, a, b, c;
    double k;
    while (scanf("%d", &n) != EOF)
    {
        if (n == 0)
            break;
        for (i = 1, a = 0, b = 0, c = 0; i <= n; i++)
        {
            scanf("%lf", &k);
            if (k < 0)
                a++;
            else if (k == 0)
                b++;
            else
                c++;
        }
        printf("%d %d %d\n", a, b, c);
    }
    return 0;
}