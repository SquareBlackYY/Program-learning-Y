// ch13.c
#include <stdio.h>

void showCh13Menu();           // 显示第13章的菜单
void pressCh13Ques(int quNum); // 运行第13章的作业

extern int return_front; // 声明return_front为外部变量

void pressCh13()
{
    int quNum;
    while (1)
    {
        showCh13Menu();
        printf("请输入题目前面的序号：");
        scanf("%d", &quNum);
        pressCh13Ques(quNum);
        if (return_front)
            break;
    }
}

void showCh13Menu()
{
    printf("第13章C语言作业（13个）\n");
    printf("0-返回上级菜单\n");
}

void pressCh13Ques(int quNum)
{
    switch (quNum)
    {
    case 0:
        return_front = 1;
        return;
    }
}