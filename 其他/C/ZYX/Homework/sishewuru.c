#include <stdio.h>
#include <math.h>
int main()
{
    for (;;)
    {
        double Answer, TrueAnswer;
        scanf("%lf %lf", &Answer, &TrueAnswer);
        if (fabs(Answer - TrueAnswer) - 0.005 < 0.001)
            printf("yes1\n");
        if (Answer >= TrueAnswer)
            printf("yes2\n");
        if (fabs(Answer - TrueAnswer) <= 0.005 && TrueAnswer - Answer != 0.05)
            printf("yes3\n");
        printf("%f\n", fabs(Answer - TrueAnswer));
    }
}