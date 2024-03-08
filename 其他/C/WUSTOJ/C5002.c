#include <stdio.h>
int main()
{
    int x, y, z;
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
    printf("%d", y);
    return 0;
}