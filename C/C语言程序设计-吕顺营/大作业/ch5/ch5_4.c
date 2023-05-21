// ch5_4.c

#include <stdio.h>

void ch5_4()
{
    float principal, amount, rate;
    int years;

    printf("请输入存钱的本金：");
    scanf("%f", &principal);
    printf("请输入存钱的期限：");
    scanf("%d", &years);

    if (years == 1)
        rate = 2.25;
    else if (years == 2)
        rate = 2.43;
    else if (years == 3)
        rate = 2.7;
    else if (years == 5)
        rate = 2.88;
    else if (years == 8)
        rate = 3.0;
    else
    {
        printf("无对应年利率！\n");
        return;
    }

    amount = principal * (1 + rate / 100 * years);

    printf("利息为：%.2f，本金利息合计为：%.2f\n", amount - principal, amount);
}
