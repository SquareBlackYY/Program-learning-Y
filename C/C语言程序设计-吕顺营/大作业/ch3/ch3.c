// ch3.c
#include <stdio.h>

void showCh3Menu();           // 显示第3章的菜单
void pressCh3Ques(int quNum); // 运行第3章的作业

extern int return_front; // 声明return_front为外部变量

void pressCh3()
{
    int quNum;
    while (1)
    {
        system("clear");
        showCh3Menu();
        printf("请输入题目前面的序号：");
        scanf("%d", &quNum);
        getchar();
        printf("********************\n");
        pressCh3Ques(quNum);
        if (return_front)
            break;
    }
}

void showCh3Menu()
{
    printf("********************\n");
    printf("第3章C语言作业（4个）\n");
    printf("********************\n");
    printf("0-返回上级菜单\n");
    printf("1-第2题 输出逆序数\n");
    printf("2-第3题 求本利和\n");
    printf("3-第4题 求解一元二次方程\n");
    printf("4-第5题 求球的体积和表面积\n");
    printf("********************\n");
}

void pressCh3Ques(int quNum)
{
    switch (quNum)
    {
    case 0:
        return_front = 1;
        return;
    case 1:
        return_front = 0;
        ch3_2();
        viewCode("./ch3/ch3_2.c");
        break;
    case 2:
        return_front = 0;
        ch3_3();
        viewCode("./ch3/ch3_3.c");
        break;
    case 3:
        return_front = 0;
        ch3_4();
        viewCode("./ch3/ch3_4.c");
        break;
    case 4:
        return_front = 0;
        ch3_5();
        viewCode("./ch3/ch3_5.c");
        break;
    }
}