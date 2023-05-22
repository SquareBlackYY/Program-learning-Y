// ch8_7.c

#include <stdio.h>
#include <stdlib.h>

int count_score(int *scores, int n, int val)
{
    int cnt = 0, i;
    for (i = 0; i < n; i++)
        if (scores[i] == val)
            cnt++;
    return cnt;
}

void print_histogram(int *scores, int n)
{
    int i, j;
    printf("Grade\tCount\tHistogram\n");
    for (i = 1; i <= 10; i++)
    {
        int cnt = count_score(scores, n, i);
        printf("%d\t%d\t", i, cnt);
        for (j = 0; j < cnt; j++)
            printf("*");
        printf("\n");
    }
}

void ch8_7()
{
    int *scores = (int *)malloc(40 * sizeof(int)), i;

    printf("请输入40个学生的打分：\n");

    for (i = 0; i < 40; i++)
        scanf("%d", &scores[i]);

    print_histogram(scores, 40);

    free(scores);
}