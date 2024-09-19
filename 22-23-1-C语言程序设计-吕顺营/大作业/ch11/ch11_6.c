// ch11_6.c

#include <stdio.h>
#include <stdlib.h>

#define N 10

void InputArray(int *pa, int m, int n)
{
    int i, j;
    printf("输入矩阵元素：");
    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            scanf("%d", pa + i * N + j);
}

int FindMax(int *pa, int m, int n, int *pRow, int *pCol)
{
    int i, j, max, row, col;
    max = *(pa);
    row = col = 0;
    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            if (*(pa + i * N + j) > max)
            {
                max = *(pa + i * N + j);
                row = i;
                col = j;
            }
    *pRow = row;
    *pCol = col;
    return max;
}

void ch11_6()
{
    int m, n, i, j, max, row, col;

    printf("请输入m*n阶矩阵的m和n：");
    scanf("%d %d", &m, &n);

    int *pa = (int *)malloc(m * n * sizeof(int));

    InputArray(pa, m, n);
    printf("输入的矩阵为：\n");
    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
            printf("%d ", *(pa + i * N + j));
        printf("\n");
    }

    max = FindMax(pa, m, n, &row, &col);

    printf("矩阵中最大的元素为%d，在第%d行第%d列.\n", max, row + 1, col + 1); // 行列下标从1开始

    free(pa);
}