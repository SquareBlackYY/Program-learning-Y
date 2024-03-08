// ch8_18.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void sortAsc(float *nums, int numsSize)
{
    int i, j;
    float temp;
    for (i = 0; i < numsSize; i++)
        for (j = i + 1; j < numsSize; j++)
            if (nums[i] > nums[j])
            {
                temp = nums[i];
                nums[i] = nums[j];
                nums[j] = temp;
            }
}

void inputScores(int m, int n, float **scores)
{
    int i, j;
    printf("请按如下顺序输入各选手%d个评委给出的分数，以空格隔开：\n", m);
    for (i = 0; i < m; i++)
    {
        scores[i] = (float *)malloc(n * sizeof(float));
        printf("第%d个选手：", i + 1);
        for (j = 0; j < n; j++)
            scanf("%f", &scores[i][j]);
    }
}

void calcFinalScore(int m, int n, float **scores, float *finalScores)
{
    int i, j, k;
    float maxScore, minScore, sum;
    for (i = 0; i < m; i++)
    {
        maxScore = minScore = scores[i][0];
        sum = scores[i][0];
        for (j = 1; j < n; j++)
        {
            if (scores[i][j] > maxScore)
                maxScore = scores[i][j];
            if (scores[i][j] < minScore)
                minScore = scores[i][j];
            sum += scores[i][j];
        }
        sum = (sum - maxScore - minScore) / (n - 2);
        finalScores[i] = sum;
    }
}

void printRanking(int m, float *finalScores)
{
    int i, j;
    float temp;
    float *tempScores = (float *)malloc(m * sizeof(float));
    memcpy(tempScores, finalScores, m * sizeof(float));
    sortAsc(tempScores, m);
    printf("按降序输出选手得分名次表：\n");
    printf("名次\t得分\n");
    for (i = m - 1; i >= 0; i--)
    {
        temp = tempScores[i];
        for (j = 0; j < m; j++)
            if (finalScores[j] == temp)
            {
                printf("%d\t%.2f\n", m - i, temp);
                break;
            }
    }
    free(tempScores);
}

void calcJudgeScore(int m, int n, float **scores, float *finalScores)
{
    int i, j, k;
    float temp, diff;
    float **judgeScores = (float **)malloc(n * sizeof(float *));
    for (i = 0; i < n; i++)
        judgeScores[i] = (float *)malloc(m * sizeof(float));
    for (i = 0; i < n; i++)
    {
        printf("请输出第%d个评委的名称：\n", i + 1);
        for (j = 0; j < m; j++)
        {
            printf("请输入%s为选手%d的评分：\n", "XXX", j + 1);
            scanf("%f", &judgeScores[i][j]);
        }
    }
    printf("各评委得分名次表：\n");
    for (i = 0; i < n; i++)
    {
        printf("第%d个评委评分：\n", i + 1);
        sortAsc(judgeScores[i], m);
        printf("名次\t得分\t评分差距\n");
        for (j = m - 1; j >= 0; j--)
        {
            temp = judgeScores[i][j];
            for (k = 0; k < m; k++)
                if (finalScores[k] == temp)
                {
                    diff = temp - finalScores[k];
                    printf("%d\t%.2f\t%.2f\n", m - j, temp, diff);
                    break;
                }
        }
    }
    for (i = 0; i < n; i++)
        free(judgeScores[i]);
    free(judgeScores);
}

void ch8_18()
{
    int m, n, i;
    printf("请输入选手人数m：\n");
    scanf("%d", &m);
    printf("请输入评委人数n（不少于3人）：\n");
    scanf("%d", &n);
    float **scores = (float **)malloc(m * sizeof(float *));
    float *finalScores = (float *)malloc(m * sizeof(float));
    inputScores(m, n, scores);
    calcFinalScore(m, n, scores, finalScores);
    printRanking(m, finalScores);
    calcJudgeScore(m, n, scores, finalScores);
    for (i = 0; i < m; i++)
        free(scores[i]);
    free(scores);
    free(finalScores);
}