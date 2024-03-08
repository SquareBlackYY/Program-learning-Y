#include<stdio.h>

void swap(int* x, int* y);

int main()
    {
        int a = 1, b = 2;
        swap(&a, &b);
        printf("%d %d",a, b);
        return 0;
    }

void swap(int* x, int* y)
{
 int t;
 t = *x;
 *x = *y;
 *y = t;
}