#include<stdio.h>

void main()
{
    char ch;
    printf("依次输入一个符号,第一行符号个数，层数：");
    int linenum,i,j,k,fst;
    scanf("%c%d%d",&ch,&fst,&linenum);
    if(linenum==1)
    //(1)
    {
        if(fst >80)
        {
        printf("您所输入的图形每行不能超过80个符号！\n");
        return;
        }
    }
    else if (linenum>1)
    {
        if(fst +linenum*2-2>80)
        {
          printf("您锁输入的图形每行不能超过80！\n");
          return;
        }
        else
        {
          for (i=1;i<=linenum;i++)
          {
            for(j=1;j<=linenum - i;j++)
              printf(" ");
            for(k = 1;k<=fst+i * 2- 2;k++)
              printf("%c", ch);
            printf("\n");
          }
        }
    }
}