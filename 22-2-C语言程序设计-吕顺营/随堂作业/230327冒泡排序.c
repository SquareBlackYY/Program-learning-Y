#include <stdio.h>

void readData(int a[], int n);
void sortData(int a[], int n, int order);
void printData(int a[], int n);
void swap(int *x, int *y);

int main()
{
    int a[1000], cnt, i, order;
    printf("请输入不超过1000个整数：");
    printf("\n你想输入几个整数？");
    scanf("%d", &cnt);
    printf("0-降序\n1-升序\n请选择：");
    scanf("%d", &order);
    readData(a, cnt);
    sortData(a, cnt, order);
    printData(a, cnt);
    return 0;
}

void readData(int a[], int n)
{
    int i;
    for (i = 0; i < n; i++)
        scanf("%d", &a[i]);
}

void sortData(int a[],int n, int order)
{
    int i, j;
    for (i = 1; i < n; i++)
        for (j = 0; j < n - i; j++)
            if (order == 0 && a[j] < a[j + 1]) //降序
                swap(&a[j], &a[j+1]);
            else if(order == 1 && a[j] > a[j + 1]) //升序
                swap(&a[j], &a[j+1]);
}

void printData(int a[], int n)
{
    int i;
    for (i = 0; i < n; i++)
        printf("%d ", a[i]);
}

void swap(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}