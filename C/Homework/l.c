#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int Calculate(int x, char op, int y);
int CreateRandomOperator(void);
int main()
{
    int answer, counter, right = 0;
    char op;
    srand(time(NULL));
    for (counter = 0; counter < 10; counter++)
    {
        
        int x = rand() % 10 + 1;
        int y = rand() % 10 + 1;
        printf("%d %d\n", x, y);
        op = CreateRandomOperator();
        printf("%d %c %d=?\n", x, op, y);
        scanf("%d", &answer);
        if (answer == Calculate(x, op, y))
        {
            printf("Right!\n");
            right++;
        }
        else
            printf("wrong!please try again\n");
    }
    printf("total score is %d\n", right * 10);
    printf("rate is %d%%\n", right * 10);
    return 0;
}
int Calculate(int x, char op, int y)
{
    switch (op)
    {
    case '+':
        return x + y;
        break;
    case '-':
        return x - y;
        break;
    case '*':
        return x * y;
        break;
    case '/':
        return x / y;
        break;
    }
}
int CreateRandomOperator(void)
{
    int op;
    op = rand() % 4 + 1;
    switch (op)
    {
    case 1:
        return '+';
        break;
    case 2:
        return '-';
        break;
    case 3:
        return '*';
        break;
    case 4:
        return '/';
        break;
    }
}