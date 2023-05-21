// ch6_22.c

#include <stdio.h>
#include <math.h>

#define CHESSBOARD_SIZE 64
#define GRAIN_PER_M3 1.42e8

void ch6_22()
{
    long long int i;
    long long int grain_sum_1 = 0, grain_sum_2; // 麦子数量的累加和

    // 直接计算累加的通项（左移运算符实现了2的n次方的计算）
    for (i = 0; i < CHESSBOARD_SIZE; i++)
        grain_sum_1 += (1 << i);

    // 利用前项计算后项
    long long int current_grain = 1;
    for (i = 1; i <= CHESSBOARD_SIZE; i++)
    {
        grain_sum_2 += current_grain;
        current_grain *= 2;
    }

    double grain_volume_1 = (double)grain_sum_1 / GRAIN_PER_M3;
    double grain_volume_2 = (double)grain_sum_2 / GRAIN_PER_M3;

    printf("直接计算累加的通项方法：舍罕王共需要%lld粒麦子，合%.2lf立方米\n", grain_sum_1, grain_volume_1);
    printf("利用前项计算后项方法：舍罕王共需要%lld粒麦子，合%.2lf立方米\n", grain_sum_2, grain_volume_2);
}
