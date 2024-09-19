// ch10_9.c

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void ch10_9()
{
    char **country = (char **)malloc(sizeof(char *) * 5);
    char temp[100] = {0};
    int i, j;

    // 输入5个国名
    for (i = 0; i < 5; i++)
    {
        country[i] = (char *)malloc(sizeof(char) * 100);
        printf("请输入第%d个国名：", i + 1);
        scanf("%s", country[i]);
    }

    // 冒泡排序，按字典顺序排列
    for (i = 0; i < 5 - 1; i++)
        for (j = 0; j < 5 - i - 1; j++)
            if (strcmp(country[j], country[j + 1]) > 0)
            {
                strcpy(temp, country[j]);
                strcpy(country[j], country[j + 1]);
                strcpy(country[j + 1], temp);
            }

    printf("按字典顺序排列后的国名为：\n");
    for (i = 0; i < 5; i++)
        puts(country[i]);

    // 释放动态分配的内存
    for (i = 0; i < 5; i++)
        free(country[i]);
    free(country);
}