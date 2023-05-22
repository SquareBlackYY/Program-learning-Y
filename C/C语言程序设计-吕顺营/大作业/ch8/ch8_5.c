// ch8_5.c

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int score;
    int id;
} Record;

int find_max(Record *arr, int n)
{
    int max_index = 0, i;
    for (i = 1; i < n; i++)
        if (arr[i].score > arr[max_index].score)
            max_index = i;
    return max_index;
}

void ch8_5()
{
    int n = 0;
    Record *records = (Record *)malloc(sizeof(Record) * 40);

    printf("请输入学生成绩和学号，以负数结束：\n");

    while (1)
    {
        int score, id;
        scanf("%d%d", &score, &id);
        if (score < 0 || id < 0)
            break;
        records[n].score = score;
        records[n].id = id;
        n++;
    }

    int max_index = find_max(records, n);

    printf("最高分为%d，学生学号为%d\n", records[max_index].score, records[max_index].id);
    free(records);
}