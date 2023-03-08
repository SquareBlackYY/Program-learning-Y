#include<stdio.h>
#include<stdlib.h>
#include<time.h>
int main()
{
    int UpperLimit, a, b, i,OperatorNum, answer, CheckAnswer, Wrong, Remainder;
    printf("除法运算时商和余数请以空格隔开\n");
    printf("请选择计算式子的范围(10,100,1000):");
    scanf ("%d",&UpperLimit);
    srand(time(NULL));
    for(;;)
    {
        for(i = 1, Wrong = 0;i <= 10;i++)
        {
            OperatorNum = rand() % 4 + 1;
            a = rand() % (UpperLimit + 1);
            for(;;)
            {
            b = rand() % (UpperLimit + 1);
            if(b==0 && OperatorNum == 4)
                continue;
            else
                break;
            }
            printf("%d", a);
            if (OperatorNum == 1)
                printf("+");
            else if (OperatorNum == 2)
                printf("-");
            else if (OperatorNum == 3)
                printf("*");
            else 
                printf("/");
            printf("%d = ", b);
            scanf("%d",&answer);
            if(OperatorNum == 4)
                scanf("%d",&Remainder);
            if( OperatorNum == 1 && answer == a + b);
            else if( OperatorNum == 2 && answer == a - b);
            else if( OperatorNum == 3 && answer == a * b);
            else if (OperatorNum == 4 && answer == a / b && Remainder == a % b);
            else
            {
                printf("做错了请再试一遍！\n");
                printf("%d", a);
                if (OperatorNum == 1)
                    printf("+");
                else if (OperatorNum == 2)
                    printf("-");
                else if (OperatorNum == 3)
                    printf("*");
                else 
                    printf("/");
                printf("%d = ", b);
                scanf("%d",&answer);
                if( OperatorNum == 1 && answer == a + b);
                else if( OperatorNum == 2 && answer == a - b);
                else if( OperatorNum == 3 && answer == a * b);
                else if (OperatorNum == 4 && answer == a / b && Remainder == a % b);
                else
                {
                    Wrong++;
                    printf("很遗憾，您两次都未回答正确！\n");
                }
            }
        }
    
    printf("共10题，正确%d题，错误%d题。\n", 10 - Wrong, Wrong);
    printf("是否要继续训练？（y/n）:");
    char choose;
    getchar();
    scanf("%c",&choose);
    if(choose=='y')
        continue;
    else
    {
        printf("拜拜，下次见哦！\n");
        break;
    }
    }
}