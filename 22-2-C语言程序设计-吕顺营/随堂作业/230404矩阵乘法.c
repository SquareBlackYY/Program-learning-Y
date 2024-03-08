#include <stdio.h>

void ReadMatrixSize(int *Row1, int *Col1, int *Row2, int *Col2);
void ReadandCalculate(int Row1, int Col1, int Row2, int Col2, int Matrix1[][Col1], int Matrix2[][Col2], int Matrix[][Col2]);
void PrintLine(int Line, int MaxLine, int *L1, int *L2, int *L3, int Row1, int Col1, int Row2, int Col2, int Matrix1[][Col1], int Matrix2[][Col2], int Matrix[][Col2]);

int main()
{
    int Row1, Col1, Row2, Col2, Line, MaxLine, L1 = 0, L2 = 0, L3 = 0;
    ReadMatrixSize(&Row1, &Col1, &Row2, &Col2);
    int Matrix1[Row1][Col1], Matrix2[Row2][Col2], Matrix[Row1][Col2];
    ReadandCalculate(Row1, Col1, Row2, Col2, Matrix1, Matrix2, Matrix);
    if (Row1 > Col1)
        MaxLine = Row1;
    else
        MaxLine = Col1;
    for (Line = 1; Line <= MaxLine; Line++)
        PrintLine(Line, MaxLine, &L1, &L2, &L3, Row1, Col1, Row2, Col2, Matrix1, Matrix2, Matrix);
    return 0;
}

void ReadMatrixSize(int *Row1, int *Col1, int *Row2, int *Col2)
{
    printf("请输入第1个数组的行数和列数：");
    scanf("%d %d", &*Row1, &*Col1);
    printf("请输入第2个数组的行数和列数：");
    scanf("%d %d", &*Row2, &*Col2);
}

void ReadandCalculate(int Row1, int Col1, int Row2, int Col2, int Matrix1[][Col1], int Matrix2[][Col2], int Matrix[][Col2])
{
    int i, j, k;
    printf("请输入第1个数组的所有元素：");
    for (i = 0; i < Row1; i++)
        for (j = 0; j < Col1; j++)
            scanf("%d", &Matrix1[i][j]);
    printf("请输入第2个数组的所有元素：");
    for (i = 0; i < Row2; i++)
        for (j = 0; j < Col2; j++)
            scanf("%d", &Matrix2[i][j]);
    for (i = 0; i < Row1; i++)
        for (j = 0; j < Col2; j++)
        {
            Matrix[i][j] = 0;
            for (k = 0; k < Col1; k++)
                Matrix[i][j] += Matrix1[i][k] * Matrix2[k][j];
        }
}

void PrintLine(int Line, int MaxLine, int *L1, int *L2, int *L3, int Row1, int Col1, int Row2, int Col2, int Matrix1[][Col1], int Matrix2[][Col2], int Matrix[][Col2])
{
    int i, Interval1 = (MaxLine - Row1) / 2, Interval2 = (MaxLine - Row2) / 2;
    if (Line > MaxLine - Row1 - Interval1 && *L1 < Row1)
    {
        printf("|");
        for (i = 0; i < Col1; i++)
            printf("%3d", Matrix1[*L1][i]);
        *L1 += 1;
        printf("  |");
    }
    else
    {
        printf(" ");
        for (i = 0; i < Col1; i++)
            printf("   ");
        printf("   ");
    }
    if (Line == (MaxLine + 2) / 2)
        printf(" × ");
    else
        printf("   ");
    if (Line > MaxLine - Row2 - Interval2 && *L2 < Row2)
    {
        printf("|");
        for (i = 0; i < Col2; i++)
            printf("%3d", Matrix2[*L2][i]);
        *L2 += 1;
        printf("  |");
    }
    else
    {
        printf(" ");
        for (i = 0; i < Col2; i++)
            printf("   ");
        printf("   ");
    }
    if (Line == (MaxLine + 2) / 2)
        printf(" = ");
    else
        printf("   ");
    if (Line > MaxLine - Row1 - Interval1 && *L3 < Row1)
    {
        printf("|");
        for (i = 0; i < Col2; i++)
            printf("%3d", Matrix[*L3][i]);
        *L3 += 1;
        printf("  |");
    }
    printf("\n");
}