#include<stdio.h>
int main ()
{
	char ch;
	int i, j, k, h, lineNum, res;
	do
	{
	    printf("请输入一个符号、首行符号个数和行数：");
        res=scanf("%c%d%d",&ch,&i,&lineNum);
        while(getchar() != '\n');
	}while(res<3);	    	
	if(lineNum==1)
	{
		if(i>80)
		{
			printf("超过了");
			return 0;
		}
		else if(i<=80)
			for(h=1;h<=i;h++)
				printf("%c",ch);
	}
	else if(lineNum!=1)
    {
		for(h=1;h<=lineNum;h++)
		{
			for(j=0;j<=lineNum-h;j++)
				printf(" ");
			for(k=1;k<=i+2*h-2;k++)
				printf("%c",ch);
		printf("\n");
		}
    }
}