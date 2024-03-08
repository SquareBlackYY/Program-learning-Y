#include <stdio.h>
int main()
{
    int x, y, z;
    scanf("%d", &x);
    y = x * 0.01 / 0.3048;
    z = (x * 0.01 / 0.3048 - y) * 12;
    printf("%d %d", y, z);
    return 0;
}