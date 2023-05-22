// ch13.c
#include <stdio.h>

void showCh13Menu();           // 显示第13章的菜单
void pressCh13Ques(int quNum); // 运行第13章的作业

extern int return_front; // 声明return_front为外部变量

void pressCh13()
{
    int quNum;
    while (1)
    {
        system("clear");
        showCh13Menu();
        printf("请输入题目前面的序号：");
        scanf("%d", &quNum);
        getchar();
        printf("********************\n");
        pressCh13Ques(quNum);
        if (return_front)
            break;
    }
}

void showCh13Menu()
{
    printf("********************\n");
    printf("第13章C语言作业（5个）\n");
    printf("********************\n");
    printf("0-返回上级菜单\n");
    printf("1-第1题 模拟DOS下的type命令\n");
    printf("2-第2题 利用结构体数组计算平均分,并输出为文件\n");
    printf("3-第3题 打开文件顺序查找某个学号\n");
    printf("4-第4题 复制文件\n");
    printf("5-第5题 文件追加\n");
    printf("********************\n");
}

void pressCh13Ques(int quNum)
{
    switch (quNum)
    {
    case 0:
        return_front = 1;
        return;
    case 1:
        return_front = 0;
        ch13_1();
        viewCode("./ch13/ch13_1.c");
        break;
    case 2:
        return_front = 0;
        ch13_2();
        viewCode("./ch13/ch13_2.c");
        break;
    case 3:
        return_front = 0;
        ch13_3();
        viewCode("./ch13/ch13_3.c");
        break;
    case 4:
        return_front = 0;
        ch13_4();
        viewCode("./ch13/ch13_4.c");
        break;
    case 5:
        return_front = 0;
        ch13_5();
        viewCode("./ch13/ch13_5.c");
        break;
    }
}