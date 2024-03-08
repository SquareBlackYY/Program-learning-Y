#include <stdio.h>
int main()
{
    int n, m, cnt, max, min, and;
    while (scanf("%d", &n) != EOF)
    {
        for (cnt = 0, and = 0; cnt < n; cnt++)
        {
            scanf("%d", &m);
            and += m;
            if (cnt == 0)
            {
                max = m;
                min = m;
            }
            if (m > max)
                max = m;
            if (m < min)
                min = m;
        }
        printf("%.2f\n", (double)(and - max - min) / (n - 2));
    }
    return 0;
}