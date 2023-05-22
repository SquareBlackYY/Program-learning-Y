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
        system("clear");
        showCh14Menu();
        printf("请输入题目前面的序号：");
        scanf("%d", &quNum);
        getchar();
        printf("********************\n");
        pressCh14Ques(quNum);
        if (return_front)
            break;
    }
}

void showCh14Menu()
{
    printf("********************\n");
    printf("第14章C语言作业（2个）\n");
    printf("********************\n");
    printf("0-返回上级菜单\n");
    printf("1-第1题 迷宫升级版\n");
    printf("2-第2题 Flappy bird升级版\n");
    printf("********************\n");
}

void pressCh14Ques(int quNum)
{
    switch (quNum)
    {
    case 0:
        return_front = 1;
        return;
    case 1:
        return_front = 0;
        ch14_1();
        viewCode("./ch14/ch14_1.c");
        break;
    case 2:
        return_front = 0;
        //ch14_2();
        viewCode("./ch14/ch14_2.c");
        break;
    }
}