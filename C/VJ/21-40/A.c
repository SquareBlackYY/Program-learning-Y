#include <stdio.h>
int main()
{
    int n, i, k, m;
    while (scanf("%d", &n) != EOF)
    {
        if (n == 0)
            break;
        for (i = 0, k = 0; i < n; i++)
        {
            scanf("%d", &m);
            while (m >= 100)
            {
                m -= 100;
                k++;
            }
            while (m >= 50)
            {
                m -= 50;
                k++;
            }
            while (m >= 10)
            {
                m -= 10;
                k++;
            }
            while (m >= 5)
            {
                m -= 5;
                k++;
            }
            while (m >= 2)
            {
                m -= 2;
                k++;
            }
            while (m >= 1)
            {
                m -= 1;
                k++;
            } 
        }
        printf("%d\n", k);
    }
    return 0;
}