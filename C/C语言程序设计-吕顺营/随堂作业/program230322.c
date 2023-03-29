#include <stdio.h>
int main()
{
    char ch;
    printf("请输入一个字符，判断字符类型:");
    scanf("%c", &ch);
    if (ch == 32)
        printf("该字符类型为空格\n");
    else if (48 <= ch && ch <= 57)
        printf("该字符类型为数字字符\n");
    else if (65 <= ch && ch <= 90)
        printf("该字符类型为大写字母\n");
    else if (97 <= ch && ch <= 122)
        printf("该字符类型为小写字母\n");
    else
        printf("该字符类型为其他字符\n");
    return 0;
}