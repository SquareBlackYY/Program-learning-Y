#include<stdio.h>
int main()
{
    int a, b;
    for(b = 0;;)
    {

        printf("输入一个a的值：");
        scanf("%d",&a);
        if(a <= 0)
        break;
        b += a;
    }
    printf("总和为:%d\n",b);
}