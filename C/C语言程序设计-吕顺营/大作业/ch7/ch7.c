// ch7.c
#include <stdio.h>

void showCh7Menu();           // 显示第7章的菜单
void pressCh7Ques(int quNum); // 运行第7章的作业

extern int return_front; // 声明return_front为外部变量

void pressCh7()
{
    int quNum;
    while (1)
    {
        system("clear");
        showCh7Menu();
        printf("请输入题目前面的序号：");
        scanf("%d", &quNum);
        printf("********************\n");
        pressCh7Ques(quNum);
        if (return_front)
            break;
    }
}

void showCh7Menu()
{
    printf("********************\n");
    printf("第7章C语言作业（7个）\n");
    printf("********************\n");
    printf("0-返回上级菜单\n");
    printf("********************\n");
}

void pressCh7Ques(int quNum)
{
    switch (quNum)
    {
    case 0:
        return_front = 1;
        return;
    }
}