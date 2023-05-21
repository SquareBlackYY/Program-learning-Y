/*# # # # # # # # # # # # # # # # # # # # # # # #
#	 				WARNING						#
#	This program can only be run on Linux.		#
# # # # # # # # # # # # # # # # # # # # # # # #*/
#include <stdio.h>
#include <stdlib.h>

void showMainMenu();		   // 显示主菜单
void showChapMenu(int chNum);  // 显示章菜单，并运行章中程序
void viewCode(char *fileName); // 查看源代码的函数

int return_front = 0; // return_front=1时返回上级菜单

void main()
{
	int chNum;
	while (1)
	{
		system("clear");
		showMainMenu();
		printf("请输入章节前面的序号：");
		scanf("%d", &chNum);
		printf("********************\n");
		showChapMenu(chNum);
	}
}

void showMainMenu()
{
	printf("********************\n");
	printf("张笑宇的C语言程序\n");
	printf("********************\n");
	printf("0-退出程序\n");
	printf("3-第3章作业（4个）\n");
	printf("4-第4章作业（1个）\n");
	printf("5-第5章作业（12个）\n");
	printf("6-第6章作业（21个）\n");
	printf("7-第7章作业（个）\n");
	printf("8-第8章作业（个）\n");
	printf("9-第9章作业（个）\n");
	printf("10-第10章作业（个）\n");
	printf("11-第11章作业（个）\n");
	printf("12-第12章作业（个）\n");
	printf("13-第13章作业（个）\n");
	printf("14-第14章作业（个）\n");
	printf("********************\n");
}

void showChapMenu(int chNum)
{
	switch (chNum)
	{
	case 0:
		system("clear");
		exit(0); // 退出程序
	case 3:
		pressCh3();
		break; // 第3章处理程序
	case 4:
		pressCh4();
		break; // 第4章处理程序
	case 5:
		pressCh5();
		break; // 第5章处理程序
	case 6:
		pressCh6();
		break; // 第6章处理程序
	case 7:
		pressCh7();
		break; // 第7章处理程序
	case 8:
		pressCh8();
		break; // 第8章处理程序
	case 9:
		pressCh9();
		break; // 第9章处理程序
	case 10:
		pressCh10();
		break; // 第10章处理程序
	case 11:
		pressCh11();
		break; // 第11章处理程序
	case 12:
		pressCh12();
		break; // 第12章处理程序
	case 13:
		pressCh13();
		break; // 第13章处理程序
	case 14:
		pressCh14();
		break; // 第14章处理程序
	}
}

// 查看源代码的函数
void viewCode(char *fileName)
{
	FILE *fp;
	char ch;
	printf("********************\n");
	printf("要查看源代码吗？需要请输入Y或y，否则，输入任意其他字符：");
	scanf(" %c", &ch);
	getchar();
	printf("********************\n");
	if (ch != 'Y' && ch != 'y')
		return;
	if ((fp = fopen(fileName, "r")) == NULL)
	{
		printf("打开文件失败！\n");
		return;
	}
	while ((ch = fgetc(fp)) != EOF)
		putchar(ch);
	fclose(fp);
	printf("\n********************\n");
	printf("按回车以继续...");
	getchar();
}