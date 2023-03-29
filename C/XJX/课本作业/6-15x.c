#include<stdio.h>
int main()
{
    int a, b, chicken, rabbit;
    printf("分别输入头和脚的个数：");
    scanf("%d%d", &a, &b);
    rabbit = ( b-2 * a ) / 2;
    chicken  =  a - rabbit;
    printf("兔的数量：%d\n鸡的数量：%d\n", rabbit, chicken);
}