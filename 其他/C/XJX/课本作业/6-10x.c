#include<stdio.h>
#include<math.h>
int x()
{
    int n;
    for(n=100;n<=999;n++)
    {
        if(n == pow(n / 100,3)+pow(n /10 % 10,3)+pow(n % 10,3))
        printf("%d\n",n);
    }
}


int main()
{
    int a, b, c;
    for(a = 1;a<=9;a++)
        for(b = 0;b<=9;b++)
            for(c = 0;c<=9;c++)
                if(100*a+10*b+c==a*a*a+b*b*b+c*c*c)
                    printf("100*a+10*b+c=%d\n",100*a+10*b+c);
    x();
}