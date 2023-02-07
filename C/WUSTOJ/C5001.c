#include <stdio.h>
int main()
{
    int n, i;
    scanf("%d", &n);
    int a[n];
    for (i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    a[0] = a[1];
    for (i = 1; i <= n; i++)
        if (a[i] > a[0])
            a[0] = a[i];
    printf("%d", a[0]);
    return 0;
}