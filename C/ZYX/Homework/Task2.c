#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main()
{
    srand(time(0));
    int a, b, answer, redo;
    for (redo = 0;;)
    {
        a = rand() % 10 + 1;
        b = rand() % 10 + 1;
        printf("%d*%d=?\n", a, b);
        for(;;)
        {
            scanf("%d", &answer);
            if (answer == a * b)
            {
                printf("Right!\n");
                break;
            }
            else
            {
                printf("Wrong!Please try again.\n");
                redo++;
            }
            if(redo >= 3) 
            {
            printf("Wrong!You have tried three times!Test over!\n");
            break;
            }
        if(redo >= 3) break;
    }
    return 0;
}