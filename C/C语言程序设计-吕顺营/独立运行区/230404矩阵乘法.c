#include <stdio.h>
int main()
{
    int Row1, Col1, Row2, Col2, i, j;
    printf("请输入第1个数组的行数和列数：");
    scanf("%d %d", &Row1, &Col1);
    printf("请输入第2个数组的行数和列数：");
    scanf("%d %d", &Row2, &Col2);
    int Matrix1[Row1][Col1], Matrix2[Row2][Col2];
    int Matrix[Row1][Col2];
    printf("请输入第1个数组的所有元素：");
    for(i = 0;i < Row1;i++)
        for(j = 0;j < Col1;j++)
            scanf("%d", &Matrix1[i][j]);
    printf("请输入第2个数组的所有元素：");
    for(i = 0;i < Row2;i++)
        for(j = 0;j < Col2;j++)
            scanf("%d", &Matrix2[i][j]);
    printf("数组乘法结果：\n");
    int m, n;
    for(i = 0;i < Row1;i++)
        for(j = 0;j < Col2;j++) 
            for(m = 0;m < Col1;m++)
                for(n = 0;n < Row2;n++)
                    Matrix[i][j] += Matrix1[i][m] * Matrix2[n][j];
    for(i = 0;i < Row1;i++)
    {
        for(j = 0;j < Col2;j++)
        {
            printf(" %d", Matrix[i][j]);
        }
        printf("\n");
    }
    return 0;
}