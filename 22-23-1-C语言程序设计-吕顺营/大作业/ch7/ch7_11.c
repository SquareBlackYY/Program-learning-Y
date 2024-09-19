// ch7_11.c

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void ch7_11()
{
    srand(time(NULL));

    int n = rand() % 2; // 0表示计算机先报数，1表示游戏者先报数
    int m = 30;         // 剩余数的个数
    int x;              // 游戏者报的数

    while (m > 0)
    {
        if (n == 0) // 计算机报数
        {
            if (m % 3 == 1 || m % 3 == 2) // 报1个数或2个数
            {
                int k = rand() % 2 + 1;
                printf("计算机报%d个数\n", k);
                m -= k;
                n = 1; // 游戏者报数
            }
            else // 报随机数
            {
                int k = rand() % 2 + 1;
                if (m - k < 0)
                    k = 1;
                printf("计算机报%d个数\n", k);
                m -= k;
                n = 1; // 游戏者报数
            }
        }
        else // 游戏者报数
        {
            printf("请输入您要报的数（1或2）：");
            scanf("%d", &x);
            if (x != 1 && x != 2)
            {
                printf("输入错误，请重新输入！\n");
                continue;
            }
            if (x == 1 && m % 3 != 1 || x == 2 && m % 3 != 2) // 报数不符合规则
            {
                printf("报数不符合规则，请重新输入！\n");
                continue;
            }
            m -= x;
            n = 0; // 计算机报数
        }
        printf("剩余数的个数：%d\n", m);
    }

    if (n == 0)
        printf("游戏者获胜！\n");
    else
        printf("计算机获胜！\n");
}