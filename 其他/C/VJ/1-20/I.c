#include <stdio.h>
#include <math.h>
int main()
{
    int n, m;
    double i, k, s;
    while (scanf("%d %d", &n, &m) != EOF)
    {
        for (i = n, s = 0.5; m > 1; m--)
        {
            k = pow(n, s);
            i += k;
            s /= 2.0;
        }
        printf("%.2f\n", i);
    }
    return 0;
}