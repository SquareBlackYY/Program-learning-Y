// ch8_14.c

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void ch8_14()
{
    srand(time(NULL));                            // 设置随机数种子
    int *digits = (int *)malloc(sizeof(int) * 4); // 分配动态数组
    int n;                                        // 可猜次数
    int a, b;                                     // 统计xAyB中的A和B
    int i, j, k;                                  // 循环变量
    int guess, target;                            // 用户猜测的数和目标数
    int flag;                                     // 判断猜测是否正确

    // 随机生成一个各位相异的四位数
    do
    {
        digits[0] = rand() % 9 + 1; // 第一位不能是0
        for (i = 1; i < 4; i++)
        {
            digits[i] = rand() % 10;
            for (j = 0; j < i; j++)
            {
                if (digits[i] == digits[j]) // 去除重复的数字
                    break;
            }
            if (j < i) // 若出现重复则重新生成
                i--;
        }
    } while (digits[0] == 0); // 如果首位是0则重新生成

    printf("文曲星猜数游戏！\n");
    printf("请输入您想要猜的次数：");
    scanf("%d", &n);

    target = digits[0] * 1000 + digits[1] * 100 + digits[2] * 10 + digits[3]; // 目标数

    for (i = 1; i <= n; i++)
    {
        printf("第%d次猜测：", i);
        scanf("%d", &guess);

        a = b = 0;
        flag = 0;
        for (j = 0; j < 4; j++)
        {
            int digit = guess % 10; // 猜测数的个位
            guess /= 10;
            if (digit == digits[j])
                a++;
            else
            {
                for (k = 0; k < 4; k++)
                    if (digit == digits[k])
                    {
                        b++;
                        break;
                    }
            }
            if (a == 4) // 可以结束循环
            {
                flag = 1;
                break;
            }
        }

        printf("%dA%dB\n", a, b);
        if (flag) // 猜对了
        {
            printf("Congratulations!\n");
            break;
        }
        else if (i == n) // 最后一次猜测也不对
            printf("Sorry, you haven't guessed the right number!\n");
    }

    free(digits);
    printf("正确的数字是：%d\n", target);
}