#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main()
{
    srand(time(0));
    int a, b, c;
    for (;;)
    {
        a = rand() % 10 + 1;
        b = rand() % 10 + 1;
        printf("%d*%d=?\n", a, b);
        for(;;)
        {
            scanf("%d", &c);
            if (c == a * b)
            {
                printf("Right!\n");
                break;
            }
            else
            {
                printf("Wrong!Please try again.\n");
            }
        }
    }
    return 0;
}