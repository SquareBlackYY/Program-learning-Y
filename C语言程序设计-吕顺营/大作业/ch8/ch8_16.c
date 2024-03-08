// ch8_16.c

#include <stdio.h>
#include <stdlib.h>

void ch8_16()
{
    int n = 0;   // 学生人数
    int *grades; // 用指针定义数组，动态分配内存
    int i, j, temp;

    grades = (int *)malloc(sizeof(int) * 40); // 最多不超过40人

    printf("输入每位同学的成绩（输入负数结束）：\n");

    // 读入学生成绩
    while (1)
    {
        int grade;
        scanf("%d", &grade);
        if (grade < 0 || n == 40)
            break;
        grades[n++] = grade;
    }

    // 冒泡排序
    for (i = 0; i < n - 1; i++)
        for (j = 0; j < n - i - 1; j++)
            if (grades[j] > grades[j + 1])
            {
                temp = grades[j];
                grades[j] = grades[j + 1];
                grades[j + 1] = temp;
            }

    // 输出排序后的学生成绩
    printf("学生成绩排序结果：\n");
    for (i = 0; i < n; i++)
        printf("%d ", grades[i]);

    free(grades); // 释放动态分配的内存
}