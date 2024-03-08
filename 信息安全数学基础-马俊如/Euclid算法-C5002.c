#include <stdio.h>
int main()
{
    int x, y, z;
    printf("求最大公因数，输入两个数：");
    scanf("%d %d", &x, &y);
    if (y > x)
    {
        z = x;
        x = y;
        y = z;
    }
    for (;x % y != 0;)
    {
        z = x % y;
        x = y;
        y = z;
    }
    printf("最大公因数是：%d\n", y);
    return 0;
}