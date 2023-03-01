#include<stdio.h>
#include<math.h>
int main()
{
    int a, b, c, d;
    double ave;
    printf("输入四个整数：");
    scanf("%d %d %d %d", &a, &b, &c, &d);
    ave = (a+b+c+d) / 4.0;
    printf("方差为：%.3f\n", (pow(a - ave, 2)+pow(b - ave, 2)+pow(c - ave, 2)+pow(d - ave, 2))/4);
}