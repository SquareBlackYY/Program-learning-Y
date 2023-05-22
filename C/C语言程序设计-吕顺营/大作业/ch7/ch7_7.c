// ch7_7.c

#include <stdio.h>

int Gcd1(int, int);
int Gcd2(int, int);
int Gcd3(int, int);

void ch7_7()
{
    int a, b;

    printf("请输入两个整数：");
    scanf("%d %d", &a, &b);

    printf("(1)穷举法\t最大公约数是：%d\n", Gcd1(a, b));
    printf("(2)欧几里得算法\t最大公约数是：%d\n", Gcd2(a, b));
    printf("(3)递归方法\t最大公约数是：%d\n", Gcd3(a, b));
}

int Gcd1(int a, int b)
{
    int t;
    if (a > b)
        t = b;
    else
        t = a;
    while (t > 0)
    {
        if (a % t == 0 && b % t == 0)
            return t;
        t--;
    }
}

int Gcd2(int a, int b)
{
    int r;
    while (b != 0)
    {
        r = a % b;
        a = b;
        b = r;
    }
    return a;
}

int Gcd3(int a, int b)
{
    if (a < b)
        return Gcd3(b, a);
    else if (a % b == 0)
        return b;
    else
        return Gcd3(b, a % b);
}