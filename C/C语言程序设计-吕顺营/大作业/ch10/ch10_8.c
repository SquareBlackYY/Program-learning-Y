// ch10_8.c

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *my_strcat(char *dest, const char *src)
{
    size_t dest_len = strlen(dest); // 获取目标字符串长度
    size_t src_len = strlen(src);   // 获取源字符串长度

    // 将源字符串拼接到目标字符串后面
    memcpy(dest + dest_len, src, src_len + 1);

    // 返回目标字符串地址
    return dest;
}

void ch10_8()
{
    char *str1 = (char *)malloc(sizeof(char) * 100);
    char *str2 = (char *)malloc(sizeof(char) * 100);
    
    printf("请输入字符串1：");
    scanf("%s", str1);
    printf("请输入字符串2：");
    scanf("%s", str2);

    my_strcat(str1, str2);
    printf("拼接后的字符串为：%s\n", str1);

    free(str1);
    free(str2);
}