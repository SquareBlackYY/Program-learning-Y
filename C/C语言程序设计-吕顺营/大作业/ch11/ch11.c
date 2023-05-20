// ch11.c
#include <stdio.h>

void showCh11Menu();           // 显示第11章的菜单
void pressCh11Ques(int quNum); // 运行第11章的作业

extern int return_front; // 声明return_front为外部变量

void pressCh11()
{
    int quNum;
    while (1)
    {
        system("clear");
        showCh11Menu();
        printf("请输入题目前面的序号：");
        scanf("%d", &quNum);
        printf("********************\n");
        pressCh11Ques(quNum);
        if (return_front)
            break;
    }
}

void showCh11Menu()
{
    printf("********************\n");
    printf("第11章C语言作业（11个）\n");
    printf("********************\n");
    printf("0-返回上级菜单\n");
    printf("********************\n");
}

void pressCh11Ques(int quNum)
{
    switch (quNum)
    {
    case 0:
        return_front = 1;
        return;
    }
}