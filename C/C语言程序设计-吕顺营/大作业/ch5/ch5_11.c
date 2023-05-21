// ch5_11.c

#include <stdio.h>

void ch5_11()
{
    char sex, sport, diet;
    float faHeight, moHeight, height;

    printf("请输入您的性别（输入 F 表示女性，输入 M 表示男性）：");
    scanf("%c", &sex);
    printf("请输入您的父亲身高（单位为 cm）：");
    scanf("%f", &faHeight);
    printf("请输入您的母亲身高（单位为 cm）：");
    scanf("%f", &moHeight);
    printf("是否喜爱体育锻炼（输入 Y 表示喜爱，输入 N 表示不喜爱）：");
    scanf(" %c", &sport);
    printf("是否有良好的饮食习惯（输入 Y 表示有，输入 N 表示没有）：");
    scanf(" %c", &diet);

    height = sex == 'F' ? (faHeight * 0.923 + moHeight) / 2 : (faHeight + moHeight) * 0.54;

    if (sport == 'Y')
        height *= 1.02;
    if (diet == 'Y')
        height *= 1.015;

    printf("您成年后的身高预测为：%.2fcm\n", height);
}
