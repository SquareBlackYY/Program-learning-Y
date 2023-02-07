#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main()
{
    srand(time(0));
    int a, b, answer, TrueAnswer, num;
    for (num = 0, TrueAnswer = 0; num < 10; num++)
    {
        a = rand() % 10 + 1;
        b = rand() % 10 + 1;
        printf("%d*%d=?\n", a, b);
        scanf("%d", &answer);
        if (answer == a * b)
        {
            printf("Right!\n");
            TrueAnswer++;
        }
        else
        {
            printf("Wrong!\n");
        }
    }
    printf("Total Score:%d\nAccuracy Rate:%d%%\n", TrueAnswer * 10, TrueAnswer * 10);
    return 0;
}