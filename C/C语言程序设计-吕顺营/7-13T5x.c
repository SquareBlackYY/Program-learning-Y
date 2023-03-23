#include<stdio.h>
int handsome1();
int main()
{
    int perfectnum,  sum = 0;
    printf("输入一个正整数：");
    scanf("%d", &perfectnum);
    if(sum == perfectnum)
        printf("此正整数是一个完全数\n");
    else 
        printf("此正整数不是一个完全数\n");
}
int handsome1()
{
    int i, perfectnum,sum = 0;
    for(i = 1;i <= (perfectnum/2);i++)
    {
        if(perfectnum % i == 0)
            sum += i;
    }
}