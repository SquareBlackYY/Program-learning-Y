// ch11_7.c

#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 10

void InputScores(int **p, int m, int n)
{
    int i, j;

    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
        {
            printf("请输入第%d个班的第%d个学生的成绩：", i + 1, j + 1);
            scanf("%d", &p[i][j]);
        }
}

void FindMaxScore(int **p, int m, int n, int *pMaxStu, int *pMaxScore)
{
    int i, j, max, maxStu, maxScore;
    max = **p;
    maxStu = maxScore = 0;
    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            if (p[i][j] > max)
            {
                max = p[i][j];
                maxStu = i + 1;
                maxScore = j + 1;
            }
    *pMaxStu = maxStu;
    *pMaxScore = maxScore;
}

void ch11_7()
{
    int m, n, i, j, max, maxStu, maxScore;

    printf("输入班级和学生的数量：");
    scanf("%d %d", &m, &n);

    int **p = (int **)malloc(m * sizeof(int *));
    for (i = 0; i < m; i++)
        p[i] = (int *)malloc(n * sizeof(int));

    InputScores(p, m, n);
    FindMaxScore(p, m, n, &maxStu, &maxScore);

    printf("最大的分数是%d分，是第%d个班的第%d个学生.\n", *(*(p + maxStu - 1) + maxScore - 1), maxStu, maxScore); // 行列下标从1开始

    for (i = 0; i < m; i++)
        free(p[i]);
    free(p);
}