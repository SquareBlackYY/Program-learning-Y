#include <stdio.h>
int anValue1(int n);
int anValue2(int n);
int cnt1 = 0, cnt2 = 0;
int main()
{
    int i, n, sum1 = 0, sum2 = 0;
    printf("输入一个正整数：");
    scanf("%d", &n);
    for(i = 1;i <= n;i++)
    {
        sum1 += anValue1(i);
        sum2 += anValue2(i);
    }
    printf("方法1前%d项和为：%d\n", n, sum1);
    printf("方法2前%d项和为：%d\n", n, sum2);
    printf("anValue1函数运行次数：%d\n", cnt1);
    printf("anValue2函数运行次数：%d\n", cnt2);
}
int anValue1(int n)
{
    int i, an, a1 = 1, a2 = 2;
    if(n == 1) an = 1;
    else if(n == 2) an = 2;
    else
    {
        for(i = 3;i <= n;i++)
        {
            an = 3 * a2 + 7 * a1;
            a1 = a2;
            a2 = an;                                                                                                                                                                                                                                        
        }
    }
    cnt1++;
    return an;
}
int anValue2(int n)
{
    int an;
    if(n == 1) an = 1;
    else if(n == 2) an = 2;
    else an = 3 * anValue2(n - 1) + 7 * anValue2(n - 2);
    cnt2++;
    return an;
}