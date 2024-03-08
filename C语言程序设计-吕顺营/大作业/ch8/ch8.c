// ch8.c
#include <stdio.h>

void showCh8Menu();           // 显示第8章的菜单
void pressCh8Ques(int quNum); // 运行第8章的作业

extern int return_front; // 声明return_front为外部变量

void pressCh8()
{
    int quNum;
    while (1)
    {
        system("clear");
        showCh8Menu();
        printf("请输入题目前面的序号：");
        scanf("%d", &quNum);
        getchar();
        printf("********************\n");
        pressCh8Ques(quNum);
        if (return_front)
            break;
    }
}

void showCh8Menu()
{
    printf("********************\n");
    printf("第8章C语言作业（16个）\n");
    printf("********************\n");
    printf("0-返回上级菜单\n");
    printf("3-第3题 统计不及格人数\n");
    printf("4-第4题 统计高于平均分人数\n");
    printf("5-第5题 输出最高分及学号\n");
    printf("6-第6题 大小数互换后输出数组\n");
    printf("7-第7题 输出餐饮质量服务结果\n");
    printf("8-第8题 统计反馈意见\n");
    printf("9-第9题 计算n阶方阵\n");
    printf("10-第10题 计算m*n阶矩阵A和B\n");
    printf("11-第11题 输出杨辉三角形\n");
    printf("12-第12题 兔子生崽问题\n");
    printf("13-第13题 模拟投骰子\n");
    printf("14-第14题 模拟猜数游戏\n");
    printf("15-第15题 查找并插入数组\n");
    printf("16-第16题 冒泡排序\n");
    printf("17-第17题 大数存储问题\n");
    printf("18-第18题 大奖赛现场统分\n");
    printf("********************\n");
}

void pressCh8Ques(int quNum)
{
    switch (quNum)
    {
    case 0:
        return_front = 1;
        return;
    case 3:
        return_front = 0;
        ch8_3();
        viewCode("./ch8/ch8_3.c");
        break;
    case 4:
        return_front = 0;
        ch8_4();
        viewCode("./ch8/ch8_4.c");
        break;
    case 5:
        return_front = 0;
        ch8_5();
        viewCode("./ch8/ch8_5.c");
        break;
    case 6:
        return_front = 0;
        ch8_6();
        viewCode("./ch8/ch8_6.c");
        break;
    case 7:
        return_front = 0;
        ch8_7();
        viewCode("./ch8/ch8_7.c");
        break;
    case 8:
        return_front = 0;
        ch8_8();
        viewCode("./ch8/ch8_8.c");
        break;
    case 9:
        return_front = 0;
        ch8_9();
        viewCode("./ch8/ch8_9.c");
        break;
    case 10:
        return_front = 0;
        ch8_10();
        viewCode("./ch8/ch8_10.c");
        break;
    case 11:
        return_front = 0;
        ch8_11();
        viewCode("./ch8/ch8_11.c");
        break;
    case 12:
        return_front = 0;
        ch8_12();
        viewCode("./ch8/ch8_12.c");
        break;
    case 13:
        return_front = 0;
        ch8_13();
        viewCode("./ch8/ch8_13.c");
        break;
    case 14:
        return_front = 0;
        ch8_14();
        viewCode("./ch8/ch8_14.c");
        break;
    case 15:
        return_front = 0;
        ch8_15();
        viewCode("./ch8/ch8_15.c");
        break;
    case 16:
        return_front = 0;
        ch8_16();
        viewCode("./ch8/ch8_16.c");
        break;
    case 17:
        return_front = 0;
        ch8_17();
        viewCode("./ch8/ch8_17.c");
        break;
    case 18:
        return_front = 0;
        ch8_18();
        viewCode("./ch8/ch8_18.c");
        break;
    }
}