// ch8_9.c

#include <stdio.h>
#include <stdlib.h>

int sum_diag(int **mat, int n)
{
    int sum = 0, i;
    for (i = 0; i < n; i++)
        sum += mat[i][i] + mat[i][n - i - 1];
    if (n % 2 == 1)
        sum -= mat[n / 2][n / 2];
    return sum;
}

void ch8_9()
{
    int n, i, j;

    printf("请输入矩阵的阶数：");
    scanf("%d", &n);

    int **mat = (int **)malloc(sizeof(int *) * n);

    for (i = 0; i < n; i++)
    {
        mat[i] = (int *)malloc(sizeof(int) * n);
        printf("请输入第%d行的%d个元素：", i + 1, n);
        for (j = 0; j < n; j++)
            scanf("%d", &mat[i][j]);
    }

    printf("矩阵两条对角线上的元素之和为：%d\n", sum_diag(mat, n));

    for (i = 0; i < n; i++)
        free(mat[i]);
    free(mat);
}