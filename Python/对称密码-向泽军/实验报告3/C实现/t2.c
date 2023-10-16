#include <stdio.h>
#include <stdlib.h>

void test_f(aaa)
{
    aaa = (char *)malloc(sizeof(char) * 4);
}

int main()
{
    char *aaa;
    for (int i;i <= 3;i++)
        test_f(aaa);
}