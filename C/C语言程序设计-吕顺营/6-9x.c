#include<stdio.h>
int main()
{
    int n, a, b;
    double e = 1;
    for(n = 1;;n++)
    {
        for(a=1,b=1;a <= n;a++)
        {
            b*=a;
        }
        if(1.0/b<0.00001)break;
        e+=1.0/b;
    }
    printf("e的近似值为：%.5f,累计项数为：%d\n", e, n + 1);
}