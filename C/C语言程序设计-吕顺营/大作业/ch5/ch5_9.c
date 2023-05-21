// ch5_9.c

#include <stdio.h>

void ch5_9()
{
    int score;
    char grade;

    printf("请输入一个百分制成绩：");
    scanf("%d", &score);

    if (score >= 90)
        grade = 'A';
    else if (score >= 80)
        grade = 'B';
    else if (score >= 70)
        grade = 'C';
    else if (score >= 60)
        grade = 'D';
    else
        grade = 'E';

    printf("%d--%c\n", score, grade);
}
