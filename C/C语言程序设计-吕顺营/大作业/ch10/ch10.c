// ch10.c
#include <stdio.h>

void showCh10Menu();           // 显示第10章的菜单
void pressCh10Ques(int quNum); // 运行第10章的作业

extern int return_front; // 声明return_front为外部变量

void pressCh10()
{
    int quNum;
    while (1)
    {
        system("clear");
        showCh10Menu();
        printf("请输入题目前面的序号：");
        scanf("%d", &quNum);
        printf("********************\n");
        pressCh10Ques(quNum);
        if (return_front)
            break;
    }
}

void showCh10Menu()
{
    printf("********************\n");
    printf("第10章C语言作业（10个）\n");
    printf("********************\n");
    printf("0-返回上级菜单\n");
    printf("********************\n");
}

void pressCh10Ques(int quNum)
{
    switch (quNum)
    {
    case 0:
        return_front = 1;
        return;
    }
}