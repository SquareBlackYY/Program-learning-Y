#include <stdio.h>
int main()
{
    double a, b, x;
    scanf("%lf %lf", &a, &b);
    int c;
    c = a / b;
    x = a - b * c;
    printf("%g", x);
    return 0;
}