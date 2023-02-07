#include <stdio.h>
#include <string.h>
#define Max 500

int main()
{
    int i, Length, CountC = 0;
    char String[Max];
    gets(String);
    Length = strlen(String);
    for (i = 0; i < Length; i++)
    {
        if (String[i] == ',')
            CountC++;
    }
    for (i = 0; String[i] != ','; i++)
        printf("%c", String[i]);
    printf("-%d", CountC + 1);
    return 0;
}