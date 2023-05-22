// ch7_12.c

#include <stdio.h>

void Hanoi(int n, char a, char b, char c)
{
    if (n == 1)
        printf("%c->%c\n", a, c);
    else
    {
        Hanoi(n - 1, a, c, b);
        printf("%c->%c\n", a, c);
        Hanoi(n - 1, b, a, c);
    }
}

void ch7_12()
{
    int n;

    printf("请输入圆盘的个数：");
    scanf("%d", &n);
    
    Hanoi(n, 'A', 'B', 'C');
}