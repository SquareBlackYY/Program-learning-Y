// ch8.c
#include <stdio.h>

void showCh8Menu();           // 显示第8章的菜单
void pressCh8Ques(int quNum); // 运行第8章的作业

extern int return_front; // 声明return_front为外部变量

void pressCh8()
{
    int quNum;
    while (1)
    {
        system("clear");
        showCh8Menu();
        printf("请输入题目前面的序号：");
        scanf("%d", &quNum);
        getchar();
        printf("********************\n");
        pressCh8Ques(quNum);
        if (return_front)
            break;
    }
}

void showCh8Menu()
{
    printf("********************\n");
    printf("第8章C语言作业（8个）\n");
    printf("********************\n");
    printf("0-返回上级菜单\n");
    printf("********************\n");
}

void pressCh8Ques(int quNum)
{
    switch (quNum)
    {
    case 0:
        return_front = 1;
        return;
    }
}