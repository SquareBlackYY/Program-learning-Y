#include <stdio.h>
#include <math.h>
int main()
{
    int a, b, c;
    scanf("%1d%1d%1d", &a, &b, &c);
    if (pow(a, 3) + pow(b, 3) + pow(c, 3) == a * 100 + b * 10 + c)
        printf("YES");
    else
        printf("NO");
    return 0;
}