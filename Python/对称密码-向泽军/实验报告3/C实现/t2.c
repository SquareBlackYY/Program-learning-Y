#include <stdio.h>
#include <stdlib.h>
int main()
{
    char *result = (char *)malloc(sizeof(char) * 3 + 1);
    char *str1 = "101";
    char *str2 = "001";

    for (int i = 0; i < 3; i++)
    {
        result[i] = str1[i] ^ str2[i];
        result[i] = '1' ^ '0';
    }
    
    result[3] = '\0';
    char r = '1' ^ '1' + '0';
    printf("%c", r);
}