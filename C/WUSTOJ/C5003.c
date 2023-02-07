#include <stdio.h>
int main()
{
    int n, s, i;
    scanf("%d", &n);
    for (s = 1, i = 1; i <= n; i++)
        s *= i;
    printf("%d!=%d", n, s);
    return 0;
}