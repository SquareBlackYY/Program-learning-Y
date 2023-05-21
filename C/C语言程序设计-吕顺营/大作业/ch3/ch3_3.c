// ch3_3.c

#include <stdio.h>
#include <math.h>

void ch3_3()
{
    float rate = 0.0225, capital, deposit;
    int n;

    printf("请输入您存款本金和存款期:");
    scanf("%f %d", &capital, &n);
    
    deposit = capital * pow(1 + rate, n);
    
    printf("%d年后您的本利和为:%.2f元。\n", n, deposit);
}