#include <stdio.h>
int main()
{

    int r1, c1, r2, c2, i, j, m;
    printf("请输入第一个矩阵的行数和列数：");
    scanf("%d%d", &r1, &c1);
    printf("请输入第二个矩阵的行数和列数：");
    scanf("%d%d", &r2, &c2);
    if (c1 != r2)
        printf("这两个矩阵无法相乘！\n");
    int matrxi[r1][c2], matrxi1[r1][c1], matrxi2[r2][c2];
    // 输入矩阵元素
    printf("请输入第一个矩阵的元素：\n");
    for (i = 0; i < r1; i++)
        for (j = 0; j < c1; j++)
            scanf("%d", &matrxi1[i][j]);
    printf("请输入第二个矩阵的元素：\n");
    for (i = 0; i < r2; i++)
        for (j = 0; j < c2; j++)
            scanf("%d", &matrxi2[i][j]);
    // 矩阵相乘运算
    for (i = 0; i < r1; i++)
    {
        for (j = 0; j < c2; j++)
        {
            matrxi[i][j] = 0;
            for (m = 0; m < c1; m++)
            {
                matrxi[i][j] += matrxi1[i][m] * matrxi2[m][j];
            }
        }
    }
    // 输出结果
    printf("矩阵相乘的结果是：\n");
    for (i = 0; i < r1; i++)
    {
        for (j = 0; j < c2; j++)
        {
            printf("%d ", matrxi[i][j]);
        }
        printf("\n");
    }
    return 0;
}