#include <stdio.h>
#include <math.h>
int main()
{
    int m, n, i, k;
    double sum;
    while (scanf("%d", &m) != EOF)
    {
        for (i = 0; i < m; i++)
        {
            scanf("%d", &n);
            for (sum = 0, k = 1; k <= n; k++)
                sum += (double)pow(-1, k+1) / k;
            printf("%.2f\n", sum);
        }
    }
    return 0;
}