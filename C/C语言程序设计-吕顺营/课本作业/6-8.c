#include<stdio.h>
void main()
{
    int cnt = 1, ch = 1, n = 0;
    double pai = 0;
    for(;(1.0 / cnt) >= 0.0001;cnt += 2, ch *= -1, n++)
        pai += (1.0 / cnt * ch);
    printf("π=%.4f,累加项数：%d\n", pai * 4, n);
}