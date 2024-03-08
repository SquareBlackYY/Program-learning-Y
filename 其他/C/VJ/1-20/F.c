#include <stdio.h>
int main()
{
    int n, i, x, o;
    while (scanf("%d", &n) != EOF)
    {
        for (o = 1, i = 1; i <= n; i++)
        {
            scanf("%d", &x);
            if (x % 2 != 0)
                o *= x;
        }
        printf("%d\n", o);
    }
    return 0;
}