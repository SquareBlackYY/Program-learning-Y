#include<stdio.h>
int main()
{
    char ch;
    int LineNum, i, j, k;
    printf("输入符号和行数：");
    scanf("%c %d",&ch, &LineNum);
    for(i=1;i<=LineNum;i++)
    {
        for(j = 0;j<=LineNum-i;j++)
            printf(" ");
        for(k = 1;k<=2*i-1;k++)
            printf("%c", ch);
        printf("\n");
    }
}