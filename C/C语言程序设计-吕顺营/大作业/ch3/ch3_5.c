// ch3_5.c
#include <stdio.h>
const double pai = 3.14;
void ch3_5()
{
    double r, s, v;
    printf("输入球的半径r：");
    scanf("%lf", &r);
    s = 4 * pai * r * r;
    v = 4.0 / 3 * pai * r * r * r;
    printf("球的表面积s=%.2f, 体积v=%.2f\n", s, v);
}