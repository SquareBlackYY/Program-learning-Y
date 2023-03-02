#include<stdio.h>
#include<stdlib.h>
#include<time.h>
int main()
{
    srand(time(NULL));
    int a, b = 0;
    for(;b<=10;b++)
    {
    a = rand()%100;
    printf("%d\n",a);
    }
}