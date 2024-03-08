#include <stdio.h>
int main()
{
    int n;
    scanf("%d", &n);
    double s;
    s = 1.0 - 1.0 / (n + 1.0);
    printf("%.5f", s);
    return 0;
}