// ch5_7.c

#include <stdio.h>

void ch5_7()
{
    char letter;

    printf("请输入一个英文字母：");
    scanf("%c", &letter);

    if (letter >= 'A' && letter <= 'Z')
        letter += 32;
    else if (letter >= 'a' && letter <= 'z')
        letter -= 32;
    
    printf("转换后字符为：%c, ASCII码为：%d\n", letter, letter);
}