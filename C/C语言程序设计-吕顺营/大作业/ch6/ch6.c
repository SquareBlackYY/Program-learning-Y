// ch6.c
#include <stdio.h>

void showCh6Menu();           // 显示第6章的菜单
void pressCh6Ques(int quNum); // 运行第6章的作业

extern int return_front; // 声明return_front为外部变量

void pressCh6()
{
    int quNum;
    while (1)
    {
        system("clear");
        showCh6Menu();
        printf("请输入题目前面的序号：");
        scanf("%d", &quNum);
        getchar();
        printf("********************\n");
        pressCh6Ques(quNum);
        if (return_front)
            break;
    }
}

void showCh6Menu()
{
    printf("********************\n");
    printf("第6章C语言作业（6个）\n");
    printf("********************\n");
    printf("0-返回上级菜单\n");
    printf("********************\n");
}

void pressCh6Ques(int quNum)
{
    switch (quNum)
    {
    case 0:
        return_front = 1;
        return;
    }
}