// ch10_5.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void deleteChar1(char str[], char ch)
{
    int i, j;
    for (i = 0, j = 0; str[i] != '\0'; i++)
        if (str[i] != ch)
            str[j++] = str[i];
    str[j] = '\0';
}

void deleteChar2(char *str, char ch)
{
    char *p = str;
    while (*str != '\0')
    {
        if (*str != ch)
            *p++ = *str;
        str++;
    }
    *p = '\0';
}

void ch10_5()
{
    //字符数组
    {
    char str[100], ch;
    printf("[字符数组]请输入字符串：");
    gets(str);
    printf("请输入要删除的字符：");
    scanf("%c", &ch);
    getchar();
    deleteChar1(str, ch);
    printf("删除字符后的字符串为：%s\n", str);
    }
    
    //函数指针
    {
    char *str = (char *) malloc(100 * sizeof(char));
    char ch;
    printf("[函数指针]请输入字符串：");
    gets(str);
    printf("请输入要删除的字符：");
    scanf("%c", &ch);
    getchar();
    deleteChar2(str, ch);
    printf("删除字符后的字符串为：%s\n", str);
    free(str);
    }
}