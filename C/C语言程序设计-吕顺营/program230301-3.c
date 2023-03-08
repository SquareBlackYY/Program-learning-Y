#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>

#define NumOfQuestion 10 //默认每次生成题数
#define LowerLimit 0 //默认随机数下限
#define NumOfReform 0 //默认单题允许重做次数
#define AdvancedDivisionMode false //高级除法模式开启时，请四舍五入保留两位小数
#define AnswerLimit true //答案范围限制，开启后答案范围将沿用随机数范围
#define ShowAnswer true //回答错误没有继续答题机会后展示正确答案

int main()
{
    int x, y, UpperLimit, i, Reform, Right, Answer, Remainder, TrueAnswer, OperatorNum, CheckAnswer;
    double TrueAdvancedDivisionModeAnswer, AdvancedDivisionModeAnswer;
    char Operator, Choice;
    printf("四则运算学习程序\n");
    if(AdvancedDivisionMode == true)
        printf("请注意：在进行除法运算时，请四舍五入保留2位小数\n");
    else
        printf("请注意：在进行除法运算时，请回答商数和余数并用空格隔开\n");
    printf("测试题数：%d\n", NumOfQuestion);
    for(;;)
    {
        printf("请选择运算范围上限(10、100、1000)：");
        for(;;)
        {
            scanf("%d", &UpperLimit);·
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
                OperatorNum = rand() % 4 + 1;
                if(OperatorNum == 4 && y == 0)
                    continue;
                switch (OperatorNum)
                {
                    case 1:
                        Operator = '+';
                        TrueAnswer = x + y;
                        break;
                    case 2:
                        Operator = '-';
                        TrueAnswer = x - y;
                        break;
                    case 3:
                        Operator = '*';
                        TrueAnswer = x * y;
                        break;
                    case 4:
                        Operator = '/';
                        if(AdvancedDivisionMode == false)
                            TrueAnswer = x / y;
                        else
                            TrueAdvancedDivisionModeAnswer == 1.0 * x / y;
                        break;
                }
                if(AnswerLimit == true && ((AdvancedDivisionMode == false && TrueAnswer < LowerLimit && TrueAnswer > UpperLimit) || (AdvancedDivisionMode == true && TrueAdvancedDivisionModeAnswer < LowerLimit && TrueAdvancedDivisionModeAnswer > UpperLimit)))
                    continue;
                break;
            }
            for(Reform = 0;Reform <= NumOfReform;Reform++)
            {
                if(AdvancedDivisionMode == false && Operator == '/')
                    printf("第%d题： %d %c %d = ?  余数为?\n", i, x, Operator, y);
                else
                    printf("第%d题： %d %c %d = ?\n", i, x, Operator, y);
                if(Operator == '/' && AdvancedDivisionMode == true)
                    scanf("%lf", &AdvancedDivisionModeAnswer);
                else if(Operator == '/' && AdvancedDivisionMode == false)
                    scanf("%d %d", &Answer, &Remainder);
                else
                    scanf("%d", &Answer);
                if(AdvancedDivisionMode == true && Operator == '/' && (TrueAdvancedDivisionModeAnswer == AdvancedDivisionModeAnswer || (TrueAdvancedDivisionModeAnswer - AdvancedDivisionModeAnswer > 0 && TrueAdvancedDivisionModeAnswer - AdvancedDivisionModeAnswer < 0.005) || (TrueAdvancedDivisionModeAnswer - AdvancedDivisionModeAnswer < 0 && TrueAdvancedDivisionModeAnswer - AdvancedDivisionModeAnswer >= -0.005)))
                    CheckAnswer = true;
                else if(AdvancedDivisionMode == false && Operator == '/' && Answer == TrueAnswer && Remainder == x % y)
                    CheckAnswer = true;
                else if(Operator != '/' && TrueAnswer == Answer)
                    CheckAnswer = true;
                else
                    CheckAnswer = false;
                if(CheckAnswer == true)
                {
                    printf("你是对的！真厉害！\n");
                    Right++;
                    break;
                }
                else if(NumOfReform == 0 || Reform == NumOfReform)
                {
                    printf("很遗憾做错了！再接再厉！\n");
                    if(ShowAnswer == true && AdvancedDivisionMode == true && Operator == '/')
                        printf("第%d题的正确答案是：%d %c %d = %.2f\n", i, x, Operator, y, TrueAdvancedDivisionModeAnswer);
                    else if(ShowAnswer == true && AdvancedDivisionMode == false && Operator == '/')
                        printf("第%d题的正确答案是：%d %c %d = %d  余数为%d\n", i, x, Operator, y, TrueAnswer, x % y);
                    else if(ShowAnswer == true)
                        printf("第%d题的正确答案是：%d %c %d = %d\n", i, x, Operator, y, TrueAnswer);
                    break;
                }
                else
                    printf("不要灰心，再试一次！\n");
            }
        }
        if(Right == NumOfQuestion)
            printf("完美！答对了全部%d道题！不要太骄傲哦～\n", Right);
        else if(1.0 * Right / NumOfQuestion >= 0.8)
            printf("太棒了！答对了%d道题！\n", Right);
        else if(1.0 * Right / NumOfQuestion >= 0.6)
            printf("做的不错！答对了%d道题！\n", Right);
        else if(1.0 * Right / NumOfQuestion >= 0.4)
            printf("答对了%d道题！再加把劲！\n", Right);
        else if(1.0 * Right / NumOfQuestion >= 0.2)
            printf("答对了%d道题！再认真一点！\n", Right);
        else
            printf("答对了%d道题，别灰心，有很大发展空间！\n", Right);
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
        {
            printf("拜拜！下次再见！");
            break;
        }
    }  
    return 0;
}