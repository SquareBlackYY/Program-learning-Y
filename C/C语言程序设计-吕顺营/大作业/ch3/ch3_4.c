// ch3_4.c

#include <stdio.h>
#include <math.h>

void ch3_4()
{
    double a, b, c, x1, x2;

    printf("输入一元二次方程中的a,b,c：");
    scanf("%lf %lf %lf", &a, &b, &c);

    x1 = (-b - pow(b * b - 4 * a * c, 0.5)) / 2 * a;
    x2 = (-b + pow(b * b - 4 * a * c, 0.5)) / 2 * a;
    
    printf("一元二次方程的根为：x₁=%.2f,x₂=%.2f\n", x1, x2);
}