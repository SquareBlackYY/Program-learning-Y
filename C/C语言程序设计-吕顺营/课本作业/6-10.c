#include <stdio.h>
#include <math.h>
void main()
{
    int n;
    for(n = 100;n <= 999;n++)
        if(n == pow(n / 100, 3) + pow(n / 10 % 10, 3) + pow (n % 10, 3))
            printf("%4d", n);
}