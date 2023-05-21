// ch5_3.c

#include <stdio.h>
#include <math.h>

void ch5_3()
{
    float a, b, c;
    float area, s;

    printf("输入任意三角形的三边长：");
    scanf("%f %f %f", &a, &b, &c);

    if (a + b > c && b + c > a && c + a > b)
    {
        s = (a + b + c) / 2;
        area = sqrt(s * (s - a) * (s - b) * (s - c));
        printf("可以构成三角形，三角形的面积是：%.2f\n", area);
    }
    else
        printf("不能构成三角形！\n");
}
