// ch5_12.c

#include <stdio.h>

void ch5_12()
{
    float height, weight, t;

    printf("请输入您的身高（单位为m）：");
    scanf("%f", &height);
    printf("请输入您的体重（单位为kg）：");
    scanf("%f", &weight);

    t = weight / (height * height);

    if (t < 18)
        printf("您的体型属于低体重型\n");
    else if (t >= 18 && t < 25)
        printf("您的体型属于正常体重型\n");
    else if (t >= 25 && t < 27)
        printf("您的体型属于超重体重型\n");
    else
        printf("您的体型属于肥胖型\n");
}
