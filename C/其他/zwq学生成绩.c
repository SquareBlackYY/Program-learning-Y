#include <stdio.h>

#define STU 40
#define COU 3
#define STYP 2 // 学生总分等类型

int readscore(int score[][COU], int n);
int readnum(int n);
int AverforStud(int STUN[][STYP], int n, int score[][COU]);
int AverforCourse(int score[][COU], int n, int COUN[][COU]);
int printAll(int score[][COU], int STUN[][STYP], int COUN[][COU], int n);

int main()
{
    int score[STU][COU], STUN[STU][STYP], COUN[STU + 2][COU], n;

    printf("请输入学生的总数:");
    scanf(" %d", &n);
    readnum(n);
    readscore(score, n);
    AverforStud(STUN, n, score);
    AverforCourse(score, n, COUN);
    printAll(score, STUN, COUN, n);
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

int readnum(int n)
{
    long num[STU];
    int i;
    printf("请输入学生的编号\n");
    for (i = 0; i < n; i++)
    {
        scanf(" %ld", &num[i]);
    }
}

int AverforStud(int STUN[][STYP], int n, int score[][COU])
{
    int i, j;
    for (i = 0; i <= n; i++)
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

int printAll(int score[][COU], int STUN[][STYP], int COUN[][COU], int n)
{
    int i, j, h;
    printf("Student's ID \t MT \t EN \t PH \t SUM \t AVER \n ");
    for (i = 0; i < n + 2; i++)
    {
        // printf("readnum\n");
        for (j = 0; j < COU; j++)
        {
            printf("%4d\t", score[i][j]);
            for (h = 0; h < STYP; h++)
            {
                printf("%4d\t", STUN[i][j]);
            }
        }
        printf("%d", COUN[i][j]);
    }
}