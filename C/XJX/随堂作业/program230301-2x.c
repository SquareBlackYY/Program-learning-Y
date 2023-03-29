#include<stdio.h>
int main()
{
    int a = 2 ,b, c, d=0;
    for(;a<=1000;a++)
    {
        for(c = 0,b= 2;b < a;b++)
        {
            if(a % b==0)
            {
                c++;
                break;
            }
        }
        if(c==0)
        {
            printf("%4d",a);
            d++;
            if(d % 5==0)
                printf("\n");
        }
    }
}