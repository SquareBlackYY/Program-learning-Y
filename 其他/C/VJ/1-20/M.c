#include <stdio.h>
int main()
{
    int n, m;
    while (scanf("%d", &n) != EOF)
    {
        for (m = 1; n > 1; n--)
        {
            m = (m + 1) * 2;
        }
        printf("%d\n", m);
    }
    return 0;
}