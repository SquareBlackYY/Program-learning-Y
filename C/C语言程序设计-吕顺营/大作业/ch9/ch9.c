// ch9.c
#include <stdio.h>

void showCh9Menu();           // 显示第9章的菜单
void pressCh9Ques(int quNum); // 运行第9章的作业

extern int return_front; // 声明return_front为外部变量

void pressCh9()
{
    int quNum;
    while (1)
    {
        showCh9Menu();
        printf("请输入题目前面的序号：");
        scanf("%d", &quNum);
        pressCh9Ques(quNum);
        if (return_front)
            break;
    }
}

void showCh9Menu()
{
    printf("第9章C语言作业（9个）\n");
    printf("0-返回上级菜单\n");
}

void pressCh9Ques(int quNum)
{
    switch (quNum)
    {
    case 0:
        return_front = 1;
        return;
    }
}