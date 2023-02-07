#include <stdio.h>
int main()
{
    int h, m, s;
    scanf("%d %d %d", &h, &m, &s);
    if (h >= 12)
    {
        printf("%d %d %d PM", h - 12, m, s);
    }
    else
    {
        printf("%d %d %d AM", h, m, s);
    }
}