#include <stdio.h>
int main()
{
    double x;
    while (scanf("%lf", &x) != EOF)
    {
        if (x >= 0)
            printf("%.2f\n", x);
        else
            printf("%.2f\n", x * -1);
    }
    return 0;
}