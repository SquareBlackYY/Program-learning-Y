#include <stdio.h>
#include <math.h>
int main()
{
    int m, n, c, k, i, o;
    while (scanf("%d %d", &m, &n) != EOF)
    {
        for (o = 0; m <= n; m++)
        {
            for (i = 1, c = 0, k = m; i <= 3; i++)
            {
                c += pow(k % 10, 3);
                k /= 10;
            }
            if (c == m)
            {
                if (o > 0)
                    printf(" ");
                printf("%d", c);
                o++;
            }
        }
        if (o == 0)
            printf("no");
        printf("\n");
    }
    return 0;
}