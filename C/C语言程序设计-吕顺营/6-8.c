#include<stdio.h>
int main()
{
    double pai = 0;
    int cnt = 1, fh = 1, n = 0;
    for(;(1.0 / cnt) >= 0.0001;cnt += 2, fh *= -1, n++)
        pai += (1.0 / cnt * fh);
    printf("π=%.4f,累加项数：%d\n", pai * 4, n);
    return 0;
}