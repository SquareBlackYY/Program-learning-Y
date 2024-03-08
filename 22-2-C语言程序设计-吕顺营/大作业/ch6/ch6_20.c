// ch6_20.c

#include <stdio.h>
#include <math.h>

void ch6_20()
{
    int num, i, j;
    
    for (i = 1; i <= 9; i++)
        for (j = 0; j <= 9; j++)
        {
            if (i == j) // 判断甲的条件
                continue;
            num = i * 1000 + i * 100 + j * 10 + j;
            if (num % 100 == i * 11) // 判断乙的条件
            {
                int sqrt_num = sqrt(num);
                if (sqrt_num * sqrt_num == num) // 判断丙的条件
                    printf("车号为：%d\n", num);
            }
        }
}