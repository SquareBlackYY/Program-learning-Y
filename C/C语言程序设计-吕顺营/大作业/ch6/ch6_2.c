// ch6_2.c

#include <stdio.h>
#include <math.h>

int ch6_2_1(void);
int ch6_2_2(void);
int ch6_2_3(void);
int ch6_2_4(void);
int ch6_2_5(void);

void ch6_2()
{
    ch6_2_1();
    ch6_2_2();
    ch6_2_3();
    ch6_2_4();
    ch6_2_5();
}

int ch6_2_1(void)
{
    int i, sum = 0;
    for (i = 1; i <= 101; i += 2)
    {
        sum += i;
    }
    printf("(1)sum=%d\n", sum);
    return 0;
}

int ch6_2_2(void)
{
    long i;
    long term, sum = 0;
    for (i = 1; i <= 101; i += 2)
    {
        term = i * (i + 1) * (i + 2);
        sum += term;
    }
    printf("(2)sum=%ld\n", sum);
    return 0;
}

int ch6_2_3(void)
{
    long i, sum = 0;
    int a, n, term = 0;
    printf("Input a,n:");
    scanf("%d %d", &a, &n);
    for (i = 1; i <= n; i++)
    {
        term = term * 10 + a;
        sum += term;
    }
    printf("(3)sum=%ld\n", sum);
    return 0;
}

int ch6_2_4(void)
{
    int n = 1;
    float term = 1.0, sign = 1, sum = 0;
    while (fabs(term) > 1e-4)
    {
        sum += sign * term;
        sign = -sign;
        n++;
        term = 1.0 / n;
    }
    printf("(4)sum=%f\n", sum);
    return 0;
}

int ch6_2_5(void)
{
    int n = 1, count = 1;
    double x;
    double sum, term;
    printf("Input x:");
    scanf("%lf", &x);
    sum = x;
    term = x;
    do
    {
        n = n + 2;
        term = term * (-x * x) / (n - 1) / n;
        sum = sum + term;
        count++;
    } while (fabs(term) > 1e-5);
    printf("(5)sin(x)=%f, count=%d\n", sum, count);
    return 0;
}