// ch5.c
#include <stdio.h>

void showCh5Menu();           // 显示第5章的菜单
void pressCh5Ques(int quNum); // 运行第5章的作业

extern int return_front; // 声明return_front为外部变量

void pressCh5()
{
    int quNum;
    while (1)
    {
        showCh5Menu();
        printf("请输入题目前面的序号：");
        scanf("%d", &quNum);
        pressCh5Ques(quNum);
        if (return_front)
            break;
    }
}

void showCh5Menu()
{
    printf("第5章C语言作业（5个）\n");
    printf("0-返回上级菜单\n");
}

void pressCh5Ques(int quNum)
{
    switch (quNum)
    {
    case 0:
        return_front = 1;
        return;
    }
}