// ch10_4.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int countWords(char *str)
{
    int count = 0, i, ifchar = 0;
    for (i = 0; str[i] != '\0'; i++)
        if (str[i] == ' ')
        {
            if(ifchar == 1)
            {
                count++;
                ifchar = 0;
            }
        }
        else
        {
            ifchar = 1;
        }
    return count + 1;
}

void ch10_4()
{
    char *str = (char *)malloc(100 * sizeof(char));

    printf("输入一行字符：");
    gets(str);

    int count = countWords(str);

    printf("该行共有 %d 个单词\n", count);

    free(str);
}