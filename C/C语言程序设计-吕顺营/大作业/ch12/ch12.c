// ch12.c
#include <stdio.h>

void showCh12Menu();           // 显示第12章的菜单
void pressCh12Ques(int quNum); // 运行第12章的作业

extern int return_front; // 声明return_front为外部变量

void pressCh12()
{
    int quNum;
    while (1)
    {
        system("clear");
        showCh12Menu();
        printf("请输入题目前面的序号：");
        scanf("%d", &quNum);
        getchar();
        printf("********************\n");
        pressCh12Ques(quNum);
        if (return_front)
            break;
    }
}

void showCh12Menu()
{
    printf("********************\n");
    printf("第12章C语言作业（5个）\n");
    printf("********************\n");
    printf("0-返回上级菜单\n");
    printf("2-第2题 时钟模拟升级版\n");
    printf("3-第3题 时钟模拟Pro Plus Ultra版\n");
    printf("4-第4题 统计得票数\n");
    printf("5-第5题 扑克牌表示\n");
    printf("6-第6题 计算逆波兰表达式\n");
    printf("********************\n");
}

void pressCh12Ques(int quNum)
{
    switch (quNum)
    {
    case 0:
        return_front = 1;
        return;
    case 2:
        return_front = 0;
        ch12_2();
        viewCode("./ch12/ch12_2.c");
        break;
    case 3:
        return_front = 0;
        ch12_3();
        viewCode("./ch12/ch12_3.c");
        break;
    case 4:
        return_front = 0;
        ch12_4();
        viewCode("./ch12/ch12_4.c");
        break;
    case 5:
        return_front = 0;
        ch12_5();
        viewCode("./ch12/ch12_5.c");
        break;
    case 6:
        return_front = 0;
        ch12_6();
        viewCode("./ch12/ch12_6.c");
        break;
    }
}