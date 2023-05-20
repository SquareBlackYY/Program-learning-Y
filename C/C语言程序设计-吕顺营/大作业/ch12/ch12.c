// ch12.c
#include <stdio.h>

void showCh12Menu();           // 显示第12章的菜单
void pressCh12Ques(int quNum); // 运行第12章的作业

extern int return_front; // 声明return_front为外部变量

void pressCh12()
{
    int quNum;
    while (1)
    {
        showCh12Menu();
        printf("请输入题目前面的序号：");
        scanf("%d", &quNum);
        pressCh12Ques(quNum);
        if (return_front)
            break;
    }
}

void showCh12Menu()
{
    printf("第12章C语言作业（12个）\n");
    printf("0-返回上级菜单\n");
}

void pressCh12Ques(int quNum)
{
    switch (quNum)
    {
    case 0:
        return_front = 1;
        return;
    }
}