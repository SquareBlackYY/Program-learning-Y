// ch6.c
#include <stdio.h>

void showCh6Menu();           // 显示第6章的菜单
void pressCh6Ques(int quNum); // 运行第6章的作业

extern int return_front; // 声明return_front为外部变量

void pressCh6()
{
    int quNum;
    while (1)
    {
        system("clear");
        showCh6Menu();
        printf("请输入题目前面的序号：");
        scanf("%d", &quNum);
        getchar();
        printf("********************\n");
        pressCh6Ques(quNum);
        if (return_front)
            break;
    }
}

void showCh6Menu()
{
    printf("********************\n");
    printf("第6章C语言作业（21个）\n");
    printf("********************\n");
    printf("0-返回上级菜单\n");
    printf("2-第2题 一些计算\n");
    printf("3-第3题 爱因斯坦数学题\n");
    printf("4-第4题 输入一些数的平方和立方\n");
    printf("5-第5题 华氏度与摄氏度转换\n");
    printf("6-第6题 计算本金\n");
    printf("7-第7题 计算产值翻番所需时间\n");
    printf("8-第8题 计算π的值\n");
    printf("9-第9题 计算e的值\n");
    printf("10-第10题 输出水仙花数\n");
    printf("11-第11题 求m的整数解\n");
    printf("12-第12题 求和（1）\n");
    printf("13-第13题 求和（2）\n");
    printf("14-第14题 马克思の趣味数学题\n");
    printf("15-第15题 鸡兔同笼问题\n");
    printf("16-第16题 百鸡问题\n");
    printf("17-第17题 兑换方案问题\n");
    printf("18-第18题 输出九九乘法表\n");
    printf("19-第19题 陌生人の换钱计划\n");
    printf("20-第20题 三个谜语人\n");
    printf("21-第21题 军舰鸣放礼炮计划\n");
    printf("22-第22题 国王の许诺\n");
    printf("********************\n");
}

void pressCh6Ques(int quNum)
{
    switch (quNum)
    {
    case 0:
        return_front = 1;
        return;
    case 2:
        return_front = 0;
        ch6_2();
        viewCode("./ch6/ch6_2.c");
        break;
    case 3:
        return_front = 0;
        ch6_3();
        viewCode("./ch6/ch6_3.c");
        break;
    case 4:
        return_front = 0;
        ch6_4();
        viewCode("./ch6/ch6_4.c");
        break;
    case 5:
        return_front = 0;
        ch6_5();
        viewCode("./ch6/ch6_5.c");
        break;
    case 6:
        return_front = 0;
        ch6_6();
        viewCode("./ch6/ch6_6.c");
        break;
    case 7:
        return_front = 0;
        ch6_7();
        viewCode("./ch6/ch6_7.c");
        break;
    case 8:
        return_front = 0;
        ch6_8();
        viewCode("./ch6/ch6_8.c");
        break;
    case 9:
        return_front = 0;
        ch6_9();
        viewCode("./ch6/ch6_9.c");
        break;
    case 10:
        return_front = 0;
        ch6_10();
        viewCode("./ch6/ch6_10.c");
        break;
    case 11:
        return_front = 0;
        ch6_11();
        viewCode("./ch6/ch6_11.c");
        break;
    case 12:
        return_front = 0;
        ch6_12();
        viewCode("./ch6/ch6_12.c");
        break;
    case 13:
        return_front = 0;
        ch6_13();
        viewCode("./ch6/ch6_13.c");
        break;
    case 14:
        return_front = 0;
        ch6_14();
        viewCode("./ch6/ch6_14.c");
        break;
    case 15:
        return_front = 0;
        ch6_15();
        viewCode("./ch6/ch6_15.c");
        break;
    case 16:
        return_front = 0;
        ch6_16();
        viewCode("./ch6/ch6_16.c");
        break;
    case 17:
        return_front = 0;
        ch6_17();
        viewCode("./ch6/ch6_17.c");
        break;
    case 18:
        return_front = 0;
        ch6_18();
        viewCode("./ch6/ch6_18.c");
        break;
    case 19:
        return_front = 0;
        ch6_19();
        viewCode("./ch6/ch6_19.c");
        break;
    case 20:
        return_front = 0;
        ch6_20();
        viewCode("./ch6/ch6_20.c");
        break;
    case 21:
        return_front = 0;
        ch6_21();
        viewCode("./ch6/ch6_21.c");
        break;
    case 22:
        return_front = 0;
        ch6_22();
        viewCode("./ch6/ch6_22.c");
        break;
    }
}