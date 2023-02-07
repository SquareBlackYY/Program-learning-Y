#include <stdio.h>

void swap(int *x, int *y);
int Search(int (*mm)(int, int), int a[]);
int min(int a, int b);
int max(int a, int b);

int main()
{
    int i, j;
    int m, n;
    int number[10] = {0};
    for (i = 0; i < 10; i++)
        scanf("%d", &number[i]);
    m = Search(min, number);
    n = Search(max, number);
    swap(number[m], number[n]);
    for (j = 0; j < 10; j++)
        printf("%d ", number[j]);
    return 0;
}

void swap(int *x, int *y)
{
    int t;
    t = *x;
    *x = *y;
    *y = t;
}

int min(int a, int b)
{
    int cnt = 0;
    if (a > b)
        cnt = 1;
    return cnt;
}

int max(int a, int b)
{
    int cnt = 0;
    if (a < b)
        cnt = 1;
    return cnt;
}

int Search(int (*mm)(int, int), int a[])
{
    int t = -1, k = a[0], i;
    int m;
    for (i = 1; i < 10; i++)
    {
        m = a[i];
        if (mm(k, m) == 1)
        {
            k = a[i];
            t = i;
        }
    }
    return t;
}