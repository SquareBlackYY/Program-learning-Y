// ch8_8.c

#include <stdio.h>
#include <stdlib.h>

int cmpfunc(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

float getMean(int *feedback, int n)
{
    int i;
    float sum = 0;
    for (i = 0; i < n; i++)
        sum += feedback[i];
    return sum / n;
}

float getMedian(int *feedback, int n)
{
    qsort(feedback, n, sizeof(int), cmpfunc);
    if (n % 2 == 0)
        return (float)(feedback[n / 2 - 1] + feedback[n / 2]) / 2;
    else
        return (float)feedback[n / 2];
}

int getMode(int *feedback, int n)
{
    int maxCount = 0, mode = 0, i, j;
    for (i = 0; i < n; i++)
    {
        int count = 0;
        for (j = 0; j < n; j++)
            if (feedback[j] == feedback[i])
                count++;
        if (count > maxCount)
        {
            maxCount = count;
            mode = feedback[i];
        }
    }
    return mode;
}

void ch8_8()
{
    int n = 40, i;
    int *feedback = (int *)malloc(n * sizeof(int));

    printf("请输入40个反馈意见：\n");

    for (i = 0; i < n; i++)
        scanf("%d", &feedback[i]);

    printf("反馈意见的平均值: %.2f\n", getMean(feedback, n));
    printf("反馈意见的中位数: %.2f\n", getMedian(feedback, n));
    printf("反馈意见的众数: %d\n", getMode(feedback, n));

    free(feedback);
}