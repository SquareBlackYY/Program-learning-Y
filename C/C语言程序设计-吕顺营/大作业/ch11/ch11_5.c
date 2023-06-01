// ch11_5.c

#include <stdio.h>
#include <stdlib.h>

#define N 10
#define M 10

void Transpose1_5(int a[][N], int at[][M], int m, int n)
{
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            at[i][j] = a[j][i];
}

void Transpose2_5(int (*a)[N], int (*at)[M], int m, int n)
{
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            at[i][j] = *((*a + j * N) + i);
}

void Transpose3_5(int *pa, int *pat, int m, int n)
{
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            *(pat + i * M + j) = *(pa + j * N + i);
}

void ch11_5()
{
    int m, n, i, j;

    printf("输入m*n阶矩阵的m和n：");
    scanf("%d %d", &m, &n);

    int *pa = (int *)malloc(m * n * sizeof(int));  // 使用malloc定义二维数组
    int *pat = (int *)malloc(m * n * sizeof(int)); // 使用malloc定义二维数组

    printf("输入矩阵的元素：");
    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            scanf("%d", pa + i * N + j);

    printf("转置前的矩阵矩阵为：\n");
    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
            printf("%d ", *(pa + i * N + j));
        printf("\n");
    }

    Transpose3_5(pa, pat, m, n);

    printf("转置后的矩阵矩阵为：\n");
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
            printf("%d ", *(pat + i * M + j));
        printf("\n");
    }

    free(pa);
    free(pat);
}