#include <stdio.h>
#include <math.h>
int main()
{
    int a, b, c;
    scanf("%d %d %d", &a, &b, &c);
    double p, s;
    p = (a + b + c) / 2;
    s = sqrt(p * (p - a) * (p - b) * (p - c));
    printf("%.2f", s);
    return 0;
}