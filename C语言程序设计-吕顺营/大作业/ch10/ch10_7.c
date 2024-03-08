// ch10_7.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reverseString1(char str[])
{
    int len = strlen(str), i;
    char temp;
    for (i = 0; i < len / 2; i++)
    {
        temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

void reverseString2(char *str)
{
    int len = strlen(str), i;
    char temp;
    for (i = 0; i < len / 2; i++)
    {
        temp = *(str + i);
        *(str + i) = *(str + len - 1 - i);
        *(str + len - 1 - i) = temp;
    }
}

void ch10_7()
{
    //字符数组
    {
        char str[100];
        printf("[字符数组]请输入字符串：");
        gets(str);
        reverseString1(str);
        printf("逆序后字符串为：%s\n", str);
    }

    //函数指针
    {
        char *str = (char *)malloc(100 * sizeof(char));
        printf("[函数指针]请输入字符串：");
        gets(str);
        reverseString2(str);
        printf("逆序后字符串为：%s\n", str);
        free(str);
    }
}