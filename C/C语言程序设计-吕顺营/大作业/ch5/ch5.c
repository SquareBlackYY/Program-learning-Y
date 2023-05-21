// ch5.c
#include <stdio.h>

void showCh5Menu();           // 显示第5章的菜单
void pressCh5Ques(int quNum); // 运行第5章的作业

extern int return_front; // 声明return_front为外部变量

void pressCh5()
{
    int quNum;
    while (1)
    {
        system("clear");
        showCh5Menu();
        printf("请输入题目前面的序号：");
        scanf("%d", &quNum);
        getchar();
        printf("********************\n");
        pressCh5Ques(quNum);
        if (return_front)
            break;
    }
}

void showCh5Menu()
{
    printf("********************\n");
    printf("第5章C语言作业（5个）\n");
    printf("********************\n");
    printf("0-返回上级菜单\n");
    printf("1-第1题 输出绝对值\n");
    printf("2-第2题 判断奇偶数\n");
    printf("3-第3题 判断三角形\n");
    printf("4-第4题 求本利和\n");
    printf("5-第5题 判断闰年\n");
    printf("6-第6题 条件表达式判断闰年\n");
    printf("7-第7题 大小写英文字母互转\n");
    printf("8-第8题 判断字符类型\n");
    printf("9-第9题 百分制成绩转五分制\n");
    printf("10-第10题 输出某年某月天数\n");
    printf("********************\n");
}

void pressCh5Ques(int quNum)
{
    switch (quNum)
    {
    case 0:
        return_front = 1;
        return;
    case 1:
        return_front = 0;
        ch5_1();
        viewCode("./ch5/ch5_1.c");
        break;
    case 2:
        return_front = 0;
        ch5_2();
        viewCode("./ch5/ch5_2.c");
        break;
    case 3:
        return_front = 0;
        ch5_3();
        viewCode("./ch5/ch5_3.c");
        break;
    case 4:
        return_front = 0;
        ch5_4();
        viewCode("./ch5/ch5_4.c");
        break;
    case 5:
        return_front = 0;
        ch5_5();
        viewCode("./ch5/ch5_5.c");
        break;
    case 6:
        return_front = 0;
        ch5_6();
        viewCode("./ch5/ch5_6.c");
        break;
    case 7:
        return_front = 0;
        ch5_7();
        viewCode("./ch5/ch5_7.c");
        break;
    case 8:
        return_front = 0;
        ch5_8();
        viewCode("./ch5/ch5_8.c");
        break;
    case 9:
        return_front = 0;
        ch5_9();
        viewCode("./ch5/ch5_9.c");
        break;
    case 10:
        return_front = 0;
        ch5_10();
        viewCode("./ch5/ch5_10.c");
        break;
    }
}