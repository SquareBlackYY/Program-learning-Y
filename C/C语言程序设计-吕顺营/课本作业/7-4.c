#include <stdio.h>
int swap(int *a, int *b)
{
    int i = *a;
    *a = *b;
    *b = i;
}
int main()
{
    int a, b, i, x, y;
    scanf("%d %d", &a, &b);
    if (a>b)
        swap(&a,&b);
    for(x = a, y = b;b % a != 0;)
    {
        i = b % a;
        swap(&a,&b);
        swap(&i,&a);
    }
    printf("最小公倍数：%d\n",x * y / a);
}