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
        getchar();
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
    printf("2-第2题 模拟数字时钟\n");
    printf("3-第3题 求最大值\n");
    printf("4-第4题 穷举法求最大公倍数\n");
    printf("5-第5题 求阶乘\n");
    printf("6-第6题 求阶乘的和\n");
    printf("7-第7题 求最大公约数\n");
    printf("8-第8题 水手分椰子\n");
    printf("9-第9题 年龄问题\n");
    printf("10-第10题 魔术师の室内互动游♂戏\n");
    printf("11-第11题 人机报数大战\n");
    printf("12-第12题 汉诺塔问题\n");
    printf("13-第13题 素数探求\n");
    printf("14-第14题 小学生计算机辅助教学系统\n");
    printf("********************\n");
}

void pressCh7Ques(int quNum)
{
    switch (quNum)
    {
    case 0:
        return_front = 1;
        return;
    case 2:
        return_front = 0;
        ch7_2();
        viewCode("./ch7/ch7_2.c");
        break;
    case 3:
        return_front = 0;
        ch7_3();
        viewCode("./ch7/ch7_3.c");
        break;
    case 4:
        return_front = 0;
        ch7_4();
        viewCode("./ch7/ch7_4.c");
        break;
    case 5:
        return_front = 0;
        ch7_5();
        viewCode("./ch7/ch7_5.c");
        break;
    case 6:
        return_front = 0;
        ch7_6();
        viewCode("./ch7/ch7_6.c");
        break;
    case 7:
        return_front = 0;
        ch7_7();
        viewCode("./ch7/ch7_7.c");
        break;
    case 8:
        return_front = 0;
        ch7_8();
        viewCode("./ch7/ch7_8.c");
        break;
    case 9:
        return_front = 0;
        ch7_9();
        viewCode("./ch7/ch7_9.c");
        break;
    case 10:
        return_front = 0;
        ch7_10();
        viewCode("./ch7/ch7_10.c");
        break;
    case 11:
        return_front = 0;
        ch7_11();
        viewCode("./ch7/ch7_11.c");
        break;
    case 12:
        return_front = 0;
        ch7_12();
        viewCode("./ch7/ch7_12.c");
        break;
    case 13:
        return_front = 0;
        ch7_13();
        viewCode("./ch7/ch7_13.c");
        break;
    case 14:
        return_front = 0;
        ch7_14();
        viewCode("./ch7/ch7_14.c");
        break;
    }
}