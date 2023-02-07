#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
int main()
{
    srand(time(0));
    int Num1, Num2, TrueAnswerNum, QuestionNum, TrueTip, FalseTip;
    double Answer, TrueAnswer;
    char Operator, OperatorNum[4] = {'+', '-', '*', '/'};
    printf("注意：在回答除法问题时请保留2位小数并四舍五入\n");
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
            if (fabs(Answer - TrueAnswer) - 0.005 < 0.001 && TrueAnswer - Answer != 0.05)
            {
                TrueTip = rand() % 4 + 1;
                switch (TrueTip)
                {
                case 1:
                    printf("Very good!\n");
                    break;
                case 2:
                    printf("Excellent!\n");
                    break;
                case 3:
                    printf("Nice work!\n");
                    break;
                case 4:
                    printf("Keep up the good work!\n");
                    break;
                }
                TrueAnswerNum++;
            }
            else
            {
                FalseTip = rand() % 4 + 1;
                switch (FalseTip)
                {
                case 1:
                    printf("No. Please try again.\n");
                    break;
                case 2:
                    printf("Wrong. Try once more.\n");
                    break;
                case 3:
                    printf("Don\'t give up!\n");
                    break;
                case 4:
                    printf("Not correct. Keep trying.\n");
                    break;
                }
            }
        }
        printf("Total Score:%d\nAccuracy Rate:%d%%\n", TrueAnswerNum * 10, TrueAnswerNum * 10);
    }
    return 0;
}