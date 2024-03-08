#include <stdio.h>
int main()
{
    int y, m, d, o, a[12];
    a[1] = 31, a[2] = 28, a[3] = 31, a[4] = 30, a[5] = 31, a[6] = 30, a[7] = 31, a[8] = 31, a[9] = 30, a[10] = 31, a[11] = 30, a[12] = 31;
    while (scanf("%d/%d/%d", &y, &m, &d) != EOF)
    {
        if ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0)
            a[2] = 29;
        else
            a[2] = 28;
        for (o = 0; m - 1 > 0; m--)
        {
            o += a[m - 1];
        }
        o += d;
        printf("%d\n", o);
    }
    return 0;
}