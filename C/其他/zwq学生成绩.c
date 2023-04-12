#include <stdio.h>

#define STU 40
#define COU 3
// #define STYP 2   //学生总分等类型

int readscore(int score[][COU], int n);
int readnum(int n, int num[]);
int AverforStud(int STUN[][COU], int n, int score[][COU]);
int AverforCourse(int score[][COU], int n, int COUN[][COU]);
int printAll(int score[][COU], int STUN[][COU], int COUN[][COU], int n, int num[]);

int main()
{
    int score[STU][COU], STUN[STU][COU], COUN[STU + 2][COU], n;
    long num[STU];
    printf("请输入学生的总数:");
    scanf(" %d", &n);
    readnum(n, num);
    readscore(score, n);
    AverforStud(STUN, n, score);
    AverforCourse(score, n, COUN);
    printAll(score, STUN, COUN, n, num);
}

int readscore(int score[][COU], int n)
{
    int i, j;
    printf("请输入学生的分数：\n");
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < COU; j++)
        {
            scanf(" %d", &score[i][j]);
        }
    }
}

int readnum(int n, int num[])
{
    int i;
    printf("请输入学生的编号\n");
    for (i = 0; i < n; i++)
    {
        scanf(" %ld", &num[i]);
    }
    return num[i];
}

int AverforStud(int STUN[][COU], int n, int score[][COU])
{
    int i, j;
    for (i = 0; i < n; i++)
    {
        STUN[i][j] = 0;
        for (j = 0; j < COU; j++)
        {
            STUN[i][j] = STUN[i][j] + score[i][j];
        }
        STUN[i][j] = STUN[i][j] / COU;
    }
}

int AverforCourse(int score[][COU], int n, int COUN[][COU])
{
    int i, j;
    for (j = 0; j < COU; j++)
    {
        COUN[i][j] = 0;
        for (i = 0; i < n; i++)
        {
            COUN[i][j] = COUN[i][j] + score[i][j];
        }
        COUN[i][j] = COUN[i][j] / n;
    }
}

int printAll(int score[][COU], int STUN[][COU], int COUN[][COU], int n, int num[])
{
    int i, j, h;
    printf("Student's ID \t MT \t EN \t PH \t SUM \t AVER \n ");
    for (i = 0; i < n; i++)
    {
        printf("%ld\t", num[i]);
        for (j = 0; j < COU; j++)
        {
            printf("%d\t", score[i][j]);
        }
        for (h = 2; h <= COU; h++)
        {
            printf("%d\t", STUN[i][h]);
        }
        printf("\n");
        if (i < n)
        {
            printf("课程总分\t");
            printf("%d", COUN[i][j]);
        }
    }
}