// ch7_13.c

#include <stdio.h>
#include <math.h>
#include <malloc.h>

int IsPrime1(int);
int IsPrime2(int);
int IsPrime3(int);
int IsPrime1a(int);
int IsPerfect(int);

void ch7_13()
{
    int m;

    printf("请输入一个整数：");
    scanf("%d", &m);

    printf("任务1：\n");

    printf("(1)使用goto语句：");
    if (IsPrime1(m))
        printf("%d is a prime number\n", m);
    else
        printf("%d is not a prime number\n", m);

    printf("(2)使用break语句：");
    if (IsPrime2(m))
        printf("%d is a prime number\n", m);
    else
        printf("%d is not a prime number\n", m);

    printf("(3)采用设置标志变量并加强循环测试：");
    if (IsPrime3(m))
        printf("%d is a prime number\n", m);
    else
        printf("%d is not a prime number\n", m);

    printf("任务2：\n");

    printf("根据素数性质：");
    if (IsPrime1a(m))
        printf("%d is a prime number\n", m);
    else
        printf("%d is not a prime number\n", m);

    printf("任务3：\n");

    int n, i, sum = 0;

    printf("请输入一个整数：");
    scanf("%d", &n);

    for (i = 2; i <= n; i++)
        if (IsPrime1a(i))
            sum += i;

    printf("1~%d之间的素数之和为：%d\n", n, sum);

    printf("任务4：\n");

    printf("请输入一个整数：");
    scanf("%d", &m);
    if (IsPrime1a(m))
        printf("No divisor! It is a prime number\n");
    else
    {
        printf("%d的所有因子为：", m);
        for (i = 2; i < m; i++)
            if (m % i == 0)
                printf("%d ", i);
        printf("\n");
    }

    printf("任务5：\n");

    printf("请输入一个整数：");
    scanf("%d", &n);

    if (IsPerfect(n))
        printf("%d是完全数\n", n);
    else
        printf("%d不是完全数\n", n);

    printf("任务6：\n");

    int j, k = 0;
    int *a;
    printf("请输入一个整数：");
    scanf("%d", &m);
    if (IsPrime1a(m))
        printf("It is a prime number\n");
    else
    {
        a = (int *)malloc(sizeof(int) * 100);
        printf("%d=", m);
        for (i = 2; i <= m; i++)
            if (IsPrime1a(i) && m % i == 0)
            {
                a[k++] = i;
                m /= i;
                i--;
            }
        for (j = 0; j < k; j++)
        {
            printf("%d", a[j]);
            if (j != k - 1)
                printf("x");
        }
        printf("\n");
        free(a);
    }
}

int IsPrime1(int m)
{
    int i = 2;
    if (m <= 1)
        return 0;
    if (m == 2)
        return 1;
    if (m % 2 == 0)
        return 0;
loop:
    if (i * i <= m)
    {
        if (m % i == 0)
            return 0;
        i++;
        goto loop;
    }
    return 1;
}

int IsPrime2(int m)
{
    int i;
    if (m <= 1)
        return 0;
    if (m == 2)
        return 1;
    if (m % 2 == 0)
        return 0;
    for (i = 3; i * i <= m; i += 2)
        if (m % i == 0)
            return 0;
    return 1;
}

int IsPrime3(int m)
{
    int i, flag = 1;
    if (m <= 1)
        return 0;
    if (m == 2)
        return 1;
    if (m % 2 == 0)
        return 0;
    for (i = 3; i * i <= m; i += 2)
        if (m % i == 0)
        {
            flag = 0;
            break;
        }
    if (flag)
        return 1;
    else
        return 0;
}

int IsPrime1a(int m)
{
    int i;
    if (m <= 1)
        return 0;
    if (m == 2)
        return 1;
    if (m % 2 == 0)
        return 0;
    for (i = 3; i <= (int)sqrt(m); i += 2)
        if (m % i == 0)
            return 0;
    return 1;
}

int IsPerfect(int n)
{
    int i, sum = 1;
    for (i = 2; i * i <= n; i++)
        if (n % i == 0)
        {
            sum += i;
            if (i * i != n)
                sum += n / i;
        }
    if (sum == n && n != 1)
        return 1;
    else
        return 0;
}