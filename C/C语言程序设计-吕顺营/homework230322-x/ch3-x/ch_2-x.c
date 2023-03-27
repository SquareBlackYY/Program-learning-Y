#include<stdio.h>
int ch3_2()
{
	int i, a[10], num;
	printf("您要输入几个数字（<=10）：");
	scanf("%d",&num);
	printf("输入这几个数字："); 
	for(i=0;i<num;i++)
		scanf("%d",&a[i]);
	for(i=num - 1;i>-1;i--)
		printf("%d ",a[i]);
	printf("\n");
}