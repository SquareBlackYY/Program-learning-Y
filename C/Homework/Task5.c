#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main()
{
    srand(time(0));
    int Num1, Num2, TrueAnswerNum, QuestionNum;
    double Answer, TrueAnswer;
    char Operator, OperatorNum[4] = {'+', '-', '*', '/'};
    for (TrueAnswerNum = 0; TrueAnswerNum < 8;)
    {
        for (QuestionNum = 0, TrueAnswerNum = 0; QuestionNum < 10; QuestionNum++)
        {
            Num1 = rand() % 10 + 1;
            Num2 = rand() % 10 + 1;
            Operator = OperatorNum[rand() % 4];
            switch (Operator)
            {
            case '+':
                TrueAnswer = Num1 + Num2;
                break;
            case '-':
                TrueAnswer = Num1 - Num2;
                break;
            case '*':
                TrueAnswer = Num1 * Num2;
                break;
            case '/':
                TrueAnswer = (Num1 * 1.0) / (Num2 * 1.0);
                break;
            }
            printf("%d%c%d=?\n", Num1, Operator, Num2);
            scanf("%lf", &Answer);
            if (Answer == TrueAnswer)
            {
                printf("Right!\n");
                TrueAnswerNum++;
            }
            else
            {
                printf("Wrong!\n");
            }
        }
        printf("Total Score:%d\nAccuracy Rate:%d%%\n", TrueAnswerNum * 10, TrueAnswerNum * 10);
    }
    return 0;
}