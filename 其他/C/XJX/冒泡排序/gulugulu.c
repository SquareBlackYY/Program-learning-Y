#include <stdio.h>
void readdata(int a[1000],int cnt, int order);
void sort(int a[], int n, int order);
void printdata(int a[],int cnt);
void swap(int j,int a[]);

int main()
{
    int a[1000], cnt, i, order;
    printf("\n你想输入几个整数？");
    scanf("%d", &cnt);
    printf("1-降序\n0-升序\n请选择：");
    scanf("%d", &order);
    readdata(a,cnt,order);
    sort(a, cnt, order);
    printdata(a, cnt);
    return 0;
}

void readdata(int a[1000],int cnt, int order)
{
    int i;
    printf("请输入不超过1000个整数：");
    for (i = 0; i < cnt; i++)
        scanf("%d", &a[i]);
}
void sort(int a[],int n, int order)
{
    int i, j, temp;
    for (i = 1; i < n; i++)
        for (j = 0; j < n - i; j++)
            if (order == 1 && a[j] < a[j + 1]) //升序
            {
               swap(j,a);
            }
            else if(order == 0 && a[j] > a[j + 1]) //降序
            {
                swap(j,a);
            }
}
void printdata(int a[], int cnt)
{
    int i;
    for (i = 0; i < cnt; i++)
    printf("%d ", a[i]);
}

void swap(int j,int a[])
{
    int temp = a[j];
    a[j] = a[j + 1];
    a[j + 1] = temp;
}