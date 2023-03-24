#include <stdio.h>
#include <stdlib.h>
void showMainMenu();/*显示主菜单*/
void showChapMenu(int chNum);/*显示章菜单，并运行章中程序*/
void viewCode(char* fileName);/*查看源代码的函数*/ 
int return_front=0; /*return_front=1时返回上级菜单*/ 
int handsome1(int perfectnum);
int pressCh3();

void main() 
{
  int chNum;
  while(1)
  {
    showMainMenu();
    printf("请输入章节前面的序号：");
    scanf("%d",&chNum);
    showChapMenu(chNum);
  }	
}

void showMainMenu()
{
  printf("吕顺营的C语言程序\n");
  printf("0-退出程序\n");
  printf("1-第3章作业（4个）\n");
  printf("2-第4章作业（5个）\n");
}

void showChapMenu(int chNum)
{
  switch(chNum)
  {
    case 0:exit(0);//退出程序 
	  case 1:pressCh3();break;//第3章处理程序
	 
	default:break;
  }
}

/*查看源代码的函数*/ 
void viewCode(char* fileName)
{
  FILE *fp;
  char ch;
  printf("要查看源代码吗？需要请输入Y或y，否则，输入任意其他字符：");
  scanf(" %c",&ch);
  if(ch!='Y'&&ch!='y')return;
  if((fp=fopen(fileName,"r"))==NULL)
  {
    printf("打开文件失败！\n");
	return;
  }

  while((ch=fgetc(fp))!=EOF)
  {
    putchar(ch);
  }
  fclose(fp);
}
int pressCh3()
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
