#include<stdio.h>
int handsome1(int perfectnum);
int main()
{
    int perfectnum;
    printf("输入一个正整数：");
    scanf("%d", &perfectnum);
    if(handsome1(perfectnum) == perfectnum)
        printf("此正整数是一个完全数\n");
    else 
        printf("此正整数不是一个完全数\n");
}
int handsome1(int perfectnum)
{
    int i, sum;
    for(i = 1,sum = 0;i <= (perfectnum/2);i++)
    {
        if(perfectnum % i == 0)
            sum += i;  
    }
    return sum;
}