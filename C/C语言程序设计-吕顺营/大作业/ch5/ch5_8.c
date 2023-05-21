// ch5_8.c

#include <stdio.h>

void ch5_8()
{
    char c;

    printf("请输入一个字符：");
    scanf("%c", &c);

    if (c >= '0' && c <= '9')
        printf("数字\n");
    else if (c >= 'A' && c <= 'Z')
        printf("大写字母\n");
    else if (c >= 'a' && c <= 'z')
        printf("小写字母\n");
    else if (c == ' ')
        printf("空格\n");
    else
        printf("其他字符\n");
}
