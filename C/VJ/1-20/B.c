#include <stdio.h>
#include <math.h>
int main()
{
    double r;
    while (scanf("%lf", &r) != EOF)
    {
        printf("%.3f\n", 4.0 / 3 * 3.1415927 * pow(r, 3));
    }
    return 0;
}