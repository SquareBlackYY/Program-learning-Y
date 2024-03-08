// ch10_6.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void insertSpace1(char str[])
{
    int len = strlen(str), i;
    for (i = 0; i < len; i++)
        printf("%c ", str[i]);
}

void insertSpace2(char *str)
{
    while (*str != '\0')
        printf("%c ", *str++);
}

void ch10_6()
{
    //字符数组
    {
        char str[100];
        printf("[字符数组]请输入字符串：");
        gets(str);
        printf("每个字符间插入空格后的字符串为：");
        insertSpace1(str);
        printf("\n");
    }

    //函数指针
    {
        char *str = (char *)malloc(100 * sizeof(char));
        printf("[函数指针]请输入字符串：");
        gets(str);
        printf("每个字符间插入空格后的字符串为：");
        insertSpace2(str);
        printf("\n");
        free(str);
    }
}