// ch8_10.c

#include <stdio.h>
#include <stdlib.h>

void ch8_10()
{
    int m, n, i, j;

    printf("请输入矩阵A的行和列数（例如：3 4）：\n");
    scanf("%d %d", &m, &n);

    int **A = (int **)malloc(m * sizeof(int *));

    printf("请按顺序输入矩阵A的%d个元素，每个元素以空格或回车隔开：\n", m * n);

    for (i = 0; i < m; i++)
    {
        A[i] = (int *)malloc(n * sizeof(int));
        for (j = 0; j < n; j++)
            scanf("%d", &A[i][j]);
    }

    printf("请输入矩阵B的行和列数（例如：3 4）：\n");
    scanf("%d %d", &m, &n);

    int **B = (int **)malloc(m * sizeof(int *));

    printf("请按顺序输入矩阵B的%d个元素，每个元素以空格或回车隔开：\n", m * n);

    for (i = 0; i < m; i++)
    {
        B[i] = (int *)malloc(n * sizeof(int));
        for (j = 0; j < n; j++)
            scanf("%d", &B[i][j]);
    }

    printf("两个矩阵之和为：\n");

    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
            printf("%d ", A[i][j] + B[i][j]);
        printf("\n");
    }
}