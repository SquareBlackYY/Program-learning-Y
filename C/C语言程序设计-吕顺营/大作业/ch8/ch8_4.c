// ch8_4.c

#include <stdio.h>
#include <stdlib.h>

float calc_avg(int *score, int n)
{
    float sum = 0;
    int i;
    for (i = 0; i < n; i++)
        sum += score[i];
    return sum / n;
}

int count_above_avg(int *score, int n)
{
    float avg = calc_avg(score, n);
    int cnt = 0, i;
    for (i = 0; i < n; i++)
        if (score[i] > avg)
            cnt++;
    return cnt;
}

void ch8_4()
{
    int n = 0;
    int *score = (int *)malloc(sizeof(int) * 40);

    printf("请输入学生成绩，以负数结束：\n");

    while (1)
    {
        int x;
        scanf("%d", &x);
        if (x < 0)
            break;
        score[n++] = x;
    }

    printf("成绩高于平均分的学生人数为：%d\n", count_above_avg(score, n));
    free(score);
}