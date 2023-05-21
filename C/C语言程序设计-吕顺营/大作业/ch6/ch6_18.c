// ch6_18.c

#include <stdio.h>

void ch6_18_1();
void ch6_18_2();
void ch6_18_3();

void ch6_18()
{
    printf("形式一：\n");
    ch6_18_1();
    printf("形式二：\n");
    ch6_18_2();
    printf("形式三：\n");
    ch6_18_3();
}

void ch6_18_1()
{
    int a, b;

    for (a = 1; a <= 9; a++)
        printf("%3d", a);
    printf("\n");

    for (a = 1; a <= 9; a++)
        printf("  -");
    printf("\n");

    for (b = 1; b <= 9; b++)
    {
        for (a = 1; a <= 9; a++)
            printf("%3d", a * b);
        printf("\n");
    }
}

void ch6_18_2()
{
    int a, b;

    for (a = 1; a <= 9; a++)
        printf("%3d", a);
    printf("\n");

    for (a = 1; a <= 9; a++)
        printf("  -");
    printf("\n");

    for (b = 1; b <= 9; b++)
    {
        for (a = 1; a <= 9; a++)
        {
            printf("%3d", a * b);
            if (a == b)
                break;
        }
        printf("\n");
    }
}

void ch6_18_3()
{
    int a, b;

    for (a = 1; a <= 9; a++)
        printf("%3d", a);
    printf("\n");

    for (a = 1; a <= 9; a++)
        printf("  -");
    printf("\n");

    for (b = 1; b <= 9; b++)
    {
        for (a = 1; a <= 9; a++)
            if (a < b)
                printf("   ");
            else
                printf("%3d", a * b);
        printf("\n");
    }
}