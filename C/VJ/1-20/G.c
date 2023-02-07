#include <stdio.h>
#include <math.h>
int main()
{
    int m, n, x, y, mid;
    while (scanf("%d %d", &m, &n) != EOF)
    {
        if (m > n)
        {
            mid = n;
            n = m;
            m = mid;
        }
        for (x = 0, y = 0; n >= m; n--)
        {
            if (n % 2 == 0)
                x += pow(n, 2);
            else
                y += pow(n, 3);
        }
        printf("%d %d\n", x, y);
    }
    return 0;
}