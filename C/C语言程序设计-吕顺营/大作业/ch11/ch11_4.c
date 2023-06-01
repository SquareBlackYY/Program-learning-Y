// ch11_4.c

#include <stdio.h>
#include <stdlib.h>

#define N 10

void Transpose1_4(int a[][N], int n)
{
    int i, j, temp;
    for (i = 0; i < n; i++)
        for (j = 0; j < i; j++)
        {
            temp = a[i][j];
            a[i][j] = a[j][i];
            a[j][i] = temp;
        }
}

void Transpose2_4(int (*pa)[N], int n)
{
    int i, j, temp;
    for (i = 0; i < n; i++)
        for (j = 0; j < i; j++)
        {
            temp = *((*pa + i * N) + j);
            *((*pa + i * N) + j) = *((*pa + j * N) + i);
            *((*pa + j * N) + i) = temp;
        }
}

void Transpose3_4(int *pa, int n)
{
    int i, j, temp;
    for (i = 0; i < n; i++)
        for (j = 0; j < i; j++)
        {
            temp = *(pa + i * N + j);
            *(pa + i * N + j) = *(pa + j * N + i);
            *(pa + j * N + i) = temp;
        }
}

void ch11_4()
{
    int n, i, j;

    printf("输入n阶矩阵的n：");
    scanf("%d", &n);

    int *pa = (int *)malloc(n * n * sizeof(int));

    printf("输入矩阵元素：");
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            scanf("%d", pa + i * N + j);

    printf("转置前的矩阵为：\n");
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
            printf("%d ", *(pa + i * N + j));
        printf("\n");
    }

    Transpose3_4(pa, n);

    printf("转置后的矩阵为：\n");
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
            printf("%d ", *(pa + i * N + j));
        printf("\n");
    }

    free(pa);
}