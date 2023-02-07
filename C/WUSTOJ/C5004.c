#include <stdio.h>
int main()
{
    int n, i;
    scanf("%d", &n);
    if (n == 2)
        printf("YES");
    else if (n == 1)
        printf("NO");
    else
    {
        for (i = 2; i < n; i++)
        {
            if (n % i == 0)
                break;
        }
        if (n == i)
            printf("YES");
        else
            printf("NO");
    }
    return 0;
}