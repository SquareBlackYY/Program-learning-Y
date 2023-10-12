#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void exchange_strings(char *str)
{
    int length = strlen(str);
    int half_length = length / 2;
    char *start = str;
    char *end = str + half_length;
    
    while (start < str + length && end < str + length)
    {
        char temp = *start;
        *start = *end;
        *end = temp;
        
        start++;
        end++;
    }
}

int main()
{
    char *aaa = (char *)malloc(sizeof(char) * 2 + 1);
    aaa[0] = 'a';
    aaa[1] = 'b';
    aaa[2] = '\0';
    puts(aaa);
    printf("%d\n", strlen(aaa));
    exchange_strings(aaa);
    puts(aaa);
    printf("%d\n", strlen(aaa));
    free(aaa);
    return 0;
}

/*
int main() {
    char str[] = "HelloWorld";
    printf("before:%s\n", str);
    
    exchange_strings(str);
    
    printf("after:%s\n", str);
    
    return 0;
}*/