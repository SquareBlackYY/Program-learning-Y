#include<stdio.h>
void xjx();
int main()
{
    int n = 1 ,s = 0;
    for(;n<=999;n++)
        s+=2 * n - 1;
    printf("%d\n",s);
    xjx();
}

void xjx()
{
    int n;
    printf("输入n的值：");
    scanf("%d",&n);
    printf("%d\n",n*n);
}