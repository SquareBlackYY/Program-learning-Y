#include <stdio.h>
#include <math.h>

void main()
{
    unsigned long long i;
    unsigned long long grain_sum_1 = 0, grain_sum_2 = 0; // 麦子数量的累加和

    // 直接计算累加的通项（左移运算符实现了2的n次方的计算）
    for (i = 0; i < 64; i++)
        grain_sum_1 += (1ull << i);

    // 利用前项计算后项
    unsigned long long current_grain = 1;
    for (i = 1; i <= 64; i++)
    {
        grain_sum_2 += current_grain;
        current_grain *= 2;
    }

    double grain_volume_1 = (double)grain_sum_1 / 1.42e8;
    double grain_volume_2 = (double)grain_sum_2 / 1.42e8;

    printf("直接计算累加的通项方法：舍罕王共需要%llu粒麦子，合%.2lf立方米\n", grain_sum_1, grain_volume_1);
    printf("利用前项计算后项方法：舍罕王共需要%llu粒麦子，合%.2lf立方米\n", grain_sum_2, grain_volume_2);
}