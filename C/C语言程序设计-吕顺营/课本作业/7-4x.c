#include <stdio.h>
int main()
{
    int a, b, i, j;
    printf("输入两个正整数：");
    scanf("%d%d", &a, &b);
    if(a<b)
        for(i = a;i>=1&&i<=a;i--)
            if(a % i == 0 && b % i == 0)
                break;
    else if(b<a)
        for(i = b;i>=1&&i<=b;i--)
            if(a % i == 0 && b % i == 0)
               break;
    printf("%d为两正整数的最小公倍数",j = (a*b)/i);
}