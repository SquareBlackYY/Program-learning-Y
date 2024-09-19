// ch8_3.c

#include <stdio.h>
#include <stdlib.h>

int count_fail(int *score, int n)
{
    int cnt = 0, i;
    for (i = 0; i < n; i++)
        if (score[i] < 60)
            cnt++;
    return cnt;
}

void ch8_3()
{
    int n, i;

    printf("请输入学生的人数：");
    scanf("%d", &n);

    int *score = (int *)malloc(sizeof(int) * n);
    printf("请输入学生成绩：\n");

    for (i = 0; i < n; i++)
        scanf("%d", &score[i]);

    printf("不及格人数为：%d\n", count_fail(score, n));
    free(score);
}