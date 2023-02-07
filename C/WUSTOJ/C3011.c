#include <stdio.h>
#include <math.h>
int main()
{
    int y;
    double l;
    scanf("%d", &y);
    l = pow(2, 1.0 / y) - 1;
    printf("%.2f%%", l * 100);
    return 0;
}