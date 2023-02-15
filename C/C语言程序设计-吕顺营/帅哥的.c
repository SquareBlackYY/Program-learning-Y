#include<stdio.h>

int main()
{
    char ch;
    printf("输入一个符号以及层数：");
    int linenum,i,j,k;
    scanf("%c%d",&ch,&linenum);
    for (i=1;i<=linenum;i++)
    {
        for(j=1;j<=linenum - i;j++)
        printf(" ");
        for(k = 1;k<=i * 2- 1;k++)
        printf("%c", ch);
    printf("\n");
    }
}