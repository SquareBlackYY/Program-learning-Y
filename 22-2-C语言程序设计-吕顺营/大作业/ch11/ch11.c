// ch11.c
#include <stdio.h>

void showCh11Menu();           // 显示第11章的菜单
void pressCh11Ques(int quNum); // 运行第11章的作业

extern int return_front; // 声明return_front为外部变量

void pressCh11()
{
    int quNum;
    while (1)
    {
        system("clear");
        showCh11Menu();
        printf("请输入题目前面的序号：");
        scanf("%d", &quNum);
        getchar();
        printf("********************\n");
        pressCh11Ques(quNum);
        if (return_front)
            break;
    }
}

void showCh11Menu()
{
    printf("********************\n");
    printf("第11章C语言作业（5个）\n");
    printf("********************\n");
    printf("0-返回上级菜单\n");
    printf("3-第3题 输出英文月份\n");
    printf("4-第4题 n*n转置矩阵\n");
    printf("5-第5题 m*n转置矩阵\n");
    printf("6-第6题 计算矩阵中元素最大值及位置\n");
    printf("7-第7题 输出最高分成绩学生信息\n");
    printf("********************\n");
}

void pressCh11Ques(int quNum)
{
    switch (quNum)
    {
    case 0:
        return_front = 1;
        return;
    case 3:
        return_front = 0;
        ch11_3();
        viewCode("./ch11/ch11_3.c");
        break;
    case 4:
        return_front = 0;
        ch11_4();
        viewCode("./ch11/ch11_4.c");
        break;
    case 5:
        return_front = 0;
        ch11_5();
        viewCode("./ch11/ch11_5.c");
        break;
    case 6:
        return_front = 0;
        ch11_6();
        viewCode("./ch11/ch11_6.c");
        break;
    case 7:
        return_front = 0;
        ch11_7();
        viewCode("./ch11/ch11_7.c");
        break;
    }
}