// ch3.c
#include <stdio.h>

void showCh3Menu();           /*显示第3章的菜单*/
void pressCh3Ques(int quNum); /*运行第3章的作业*/

extern int return_front; /*声明return_front为外部变量*/

void pressCh3()
{
    int quNum;
    while (1)
    {
        showCh3Menu();
        printf("请输入题目前面的序号：");
        scanf("%d", &quNum);
        pressCh3Ques(quNum);
        if (return_front)
            break;
    }
}

void showCh3Menu()
{
    printf("第3章C语言作业（4个）\n");
    printf("0-返回上级菜单\n");
    printf("1-第2题 输出逆序数\n");
    printf("2-第3题 求本利和\n");
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
        viewCode(".\\ch3\\ch3_2.c");
        break;
    case 2:
        return_front = 0;
        ch3_3();
        viewCode(".\\ch3\\ch3_3.c");
        break;
    }
}