// ch8_11.c

#include <stdio.h>
#include <stdlib.h>

void printYangHui(int n)
{
    int **YH = (int **)malloc(n * sizeof(int *));
    int i, j;
    
    for (i = 0; i < n; i++)
    {
        YH[i] = (int *)malloc((i + 1) * sizeof(int));
        YH[i][0] = YH[i][i] = 1;
        for (j = 1; j < i; j++)
            YH[i][j] = YH[i - 1][j - 1] + YH[i - 1][j];
        for (j = 0; j <= i; j++)
            printf("%d ", YH[i][j]);
        printf("\n");
    }

    for (i = 0; i < n; i++)
        free(YH[i]);
    free(YH);
}

void ch8_11()
{
    int n;
    printf("请输入要打印的杨辉三角形的行数：\n");
    scanf("%d", &n);
    printYangHui(n);
}