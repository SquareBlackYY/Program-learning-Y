#include<stdio.h>
int main()
{
    int a,b ;
    for (a=1;a<=9;a++)
    {
        printf("%d\t",a);
    }
    printf("\n");
    for(a=1;a<=9;a++)
    {
        printf("-\t");
    }
    printf("\n");
    for(b=1;b<=9;b++)
    {
        for(a=1;a<=b;a++)
        {
            printf("%d\t",a*b);
        }
        printf("\n");
    }
}