// ch4.c
#include <stdio.h>

void showCh4Menu();           // 显示第4章的菜单
void pressCh4Ques(int quNum); // 运行第4章的作业

extern int return_front; // 声明return_front为外部变量

void pressCh4()
{
    int quNum;
    while (1)
    {
        system("clear");
        showCh4Menu();
        printf("请输入题目前面的序号：");
        scanf("%d", &quNum);
        getchar();
        printf("********************\n");
        pressCh4Ques(quNum);
        if (return_front)
            break;
    }
}

void showCh4Menu()
{
    printf("********************\n");
    printf("第4章C语言作业（1个）\n");
    printf("********************\n");
    printf("0-返回上级菜单\n");
    printf("1-第4题 小写字母转大写并输出ASCII码\n");
    printf("********************\n");
}

void pressCh4Ques(int quNum)
{
    switch (quNum)
    {
    case 0:
        return_front = 1;
        return;
    case 1:
        return_front = 0;
        ch4_4();
        viewCode("./ch4/ch4_4.c");
        break;
    }
}