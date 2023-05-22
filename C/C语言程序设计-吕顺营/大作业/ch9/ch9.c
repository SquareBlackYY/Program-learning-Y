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
        system("clear");
        showCh9Menu();
        printf("请输入题目前面的序号：");
        scanf("%d", &quNum);
        getchar();
        printf("********************\n");
        pressCh9Ques(quNum);
        if (return_front)
            break;
    }
}

void showCh9Menu()
{
    printf("********************\n");
    printf("第9章C语言作业（5个）\n");
    printf("********************\n");
    printf("0-返回上级菜单\n");
    printf("2-第2题 返回一个数组中所有元素被第一个元素除的结果\n");
    printf("3-第3题 两个数组中对应元素值的交换\n");
    printf("4-第4题 计算其最大值和最小值，并交换位置\n");
    printf("5-第5题 日期转换问题\n");
    printf("6-第6题 计算函数定积分\n");
    printf("********************\n");
}

void pressCh9Ques(int quNum)
{
    switch (quNum)
    {
    case 0:
        return_front = 1;
        return;
    case 2:
        return_front = 0;
        ch9_2();
        viewCode("./ch9/ch9_2.c");
        break;
    case 3:
        return_front = 0;
        ch9_3();
        viewCode("./ch9/ch9_3.c");
        break;
    case 4:
        return_front = 0;
        ch9_4();
        viewCode("./ch9/ch9_4.c");
        break;
    case 5:
        return_front = 0;
        ch9_5();
        viewCode("./ch9/ch9_5.c");
        break;
    case 6:
        return_front = 0;
        ch9_6();
        viewCode("./ch9/ch9_6.c");
        break;
    }
}