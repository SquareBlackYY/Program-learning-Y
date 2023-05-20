// ch14.c
#include <stdio.h>

void showCh14Menu();           // 显示第14章的菜单
void pressCh14Ques(int quNum); // 运行第14章的作业

extern int return_front; // 声明return_front为外部变量

void pressCh14()
{
    int quNum;
    while (1)
    {
        showCh14Menu();
        printf("请输入题目前面的序号：");
        scanf("%d", &quNum);
        pressCh14Ques(quNum);
        if (return_front)
            break;
    }
}

void showCh14Menu()
{
    printf("第14章C语言作业（14个）\n");
    printf("0-返回上级菜单\n");
}

void pressCh14Ques(int quNum)
{
    switch (quNum)
    {
    case 0:
        return_front = 1;
        return;
    }
}