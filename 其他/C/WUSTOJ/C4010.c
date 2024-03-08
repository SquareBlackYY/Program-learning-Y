#include <stdio.h>
#include <math.h>
int main()
{
    double x, y, z;
    scanf("%lf", &x);
    if (x <= 2.5)
        y = x * x + 1;
    else
        y = x * x - 1;
    if (x >= 5)
        z = x * x - 2 * x + 5;
    else if (x >= 3)
        z = pow(x * x + 1, 0.5);
    else if (x >= 2)
        z = 2 * sin(x) - 1;
    else
        z = 3 * x + 5;
        printf("%.6f\n%.6f", y, z);
    return 0;
}