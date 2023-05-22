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
        getchar();
        printf("********************\n");
        pressCh10Ques(quNum);
        if (return_front)
            break;
    }
}

void showCh10Menu()
{
    printf("********************\n");
    printf("第10章C语言作业（8个）\n");
    printf("********************\n");
    printf("0-返回上级菜单\n");
    printf("3-第3题 字符串处理系列\n");
    printf("4-第4题 统计单词\n");
    printf("5-第5题 删除相同字符\n");
    printf("6-第6题 字符间插入空格\n");
    printf("7-第7题 字符串逆序存放\n");
    printf("8-第8题 字符串连接\n");
    printf("9-第9题 字符串排序实战-国名查找\n");
    printf("10-第10题 查找星期表\n");
    printf("********************\n");
}

void pressCh10Ques(int quNum)
{
    switch (quNum)
    {
    case 0:
        return_front = 1;
        return;
    case 3:
        return_front = 0;
        ch10_3();
        viewCode("./ch10/ch10_3.c");
        break;
    case 4:
        return_front = 0;
        ch10_4();
        viewCode("./ch10/ch10_4.c");
        break;
    case 5:
        return_front = 0;
        ch10_5();
        viewCode("./ch10/ch10_5.c");
        break;
    case 6:
        return_front = 0;
        ch10_6();
        viewCode("./ch10/ch10_6.c");
        break;
    case 7:
        return_front = 0;
        ch10_7();
        viewCode("./ch10/ch10_7.c");
        break;
    case 8:
        return_front = 0;
        ch10_8();
        viewCode("./ch10/ch10_8.c");
        break;
    case 9:
        return_front = 0;
        ch10_9();
        viewCode("./ch10/ch10_9.c");
        break;
    case 10:
        return_front = 0;
        ch10_10();
        viewCode("./ch10/ch10_10.c");
        break;
    }
}