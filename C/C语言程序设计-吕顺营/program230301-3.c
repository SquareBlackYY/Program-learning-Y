#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define NumOfQuestion 10 //默认每次生成题数
#define LowerLimit 0 //默认随机数下限
#define NumOfReform 0 //默认单题允许重做次数

int CreateRandomOperator(void);
int CheckAnswer(int x, char Operator, int y, double Answer);

int main()
{
    int x, y, UpperLimit, i, Reform, Right;
    double Answer;
    char Operator, Choice;
    printf("四则运算学习程序\n"
           "请注意：在进行除法运算时请四舍五入保留2位小数\n"
           "测试题数：%d\n", NumOfQuestion);
    for(;;)
    {
        printf("请选择运算范围上限(10、100、1000)：");
        for(;;)
        {
            scanf("%d", &UpperLimit);
            if(UpperLimit == 10 || UpperLimit == 100 || UpperLimit == 1000)
                break;
            printf("不符合要求！请重新选择运算范围上限(10、100、1000)：");
        }
        srand(time(NULL));
        for(i = 1, Right = 0; i <= NumOfQuestion; i++)
        {
            for(;;)
            {
                x = LowerLimit + rand() % (UpperLimit + 1 - LowerLimit);
                y = LowerLimit + rand() % (UpperLimit + 1 - LowerLimit);
                Operator = CreateRandomOperator();
                if(Operator == '/' && y == 0)
                    continue;
                break;
            }
            for(Reform = 0;Reform <= NumOfReform;Reform++)
            {
                printf("第%d题： %d %c %d = ?\n", i, x, Operator, y);
                scanf("%lf", &Answer);
                if(CheckAnswer(x, Operator, y, Answer) == 1)
                {
                    printf("你是对的！\n");
                    Right++;
                    break;
                }
                else if(NumOfReform == 0)
                    printf("很遗憾做错了！再接再厉！\n");
                else
                    printf("不要灰心，再试一次！\n");
            }
        }
        printf("太棒了！你总共答对了%d道题！\n", Right);
        for(;;)
        {
            printf("你要继续训练吗(y/n)：");
            getchar();
            scanf("%c", &Choice);
            if(Choice != 'y' && Choice != 'Y' && Choice != 'n' && Choice != 'N')
                continue;
            break;
        }
        if(Choice == 'n' || Choice == 'N')
            break;
    }
    return 0;
}

int CreateRandomOperator(void)
{
    int OperatorNum = rand() % 4 + 1;
    char Operator;
    switch (OperatorNum)
    {
        case 1:
            Operator = '+';
            break;
        case 2:
            Operator = '-';
            break;
        case 3:
            Operator = '*';
            break;
        case 4:
            Operator = '/';
            break;
    }
    return Operator;
}

int CheckAnswer(int x, char Operator, int y, double Answer)
{
    int CheckCode;
    switch (Operator)
    {
        case '+':
            if(x + y == Answer)
                CheckCode = 1;
            else
                CheckCode = 0;
            break;
        case '-':
            if(x - y == Answer)
                CheckCode = 1;
            else
                CheckCode = 0;
            break;
        case '*':
            if(x * y == Answer)
                CheckCode = 1;
            else
                CheckCode = 0;
            break;
        case '/':
            if(1.0 * x / y == Answer || (1.0 * x / y - Answer > 0 && 1.0 * x / y - Answer < 0.005) || (Answer - 1.0 * x / y > 0 && Answer - 1.0 * x / y <= 0.005))
                CheckCode = 1;
            else
                CheckCode = 0;
            break;
    }
    return CheckCode;
}