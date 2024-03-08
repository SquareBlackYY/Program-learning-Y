// ch4_4.c

#include <stdio.h>

void ch4_4()
{
    char lowerCaseLetter;

    printf("请输入一个小写英文字母：");
    scanf("%c", &lowerCaseLetter);

    char upperCaseLetter = lowerCaseLetter - 32;
    
    printf("转换后的大写英文字母为%c，ASCII码值为%d\n", upperCaseLetter, (int)upperCaseLetter);
}