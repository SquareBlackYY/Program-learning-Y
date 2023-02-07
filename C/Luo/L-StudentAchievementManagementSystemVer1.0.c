#include <stdio.h>
#include <string.h>
#define MaxData 59

int UserSelectionJudgment();
long CollectStudentIDAndScores(long StudentIDAndScores[], int *StudentNumber);
long CalculateTotalAndAverageScores(long StudentIDAndScores[], int *StudentNumber, long Total[], double Average[]);
void SelectLanguage(int *LanguageCode);
void Text(int TextCode, int *LanguageCode);

void main()
{
    int Choice = -1, Count, StudentNumber, LanguageCode = 0;
    long StudentIDAndScores[MaxData] = {0}, Total[(MaxData - 1) / 2] = {0};
    double Average[(MaxData - 1) / 2] = {0};
    SelectLanguage(&LanguageCode);
    for (;;)
    {
        Text(3, &LanguageCode);
        for (;;)
        {
            scanf("%d", &Choice);
            switch (Choice)
            {
            case 0:
                printf("See you~");
                return;
            case 1:
                CollectStudentIDAndScores(StudentIDAndScores, &StudentNumber);
                break;
            case 2:
                CalculateTotalAndAverageScores(StudentIDAndScores, &StudentNumber, Total, Average);
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            case 7:
                break;
            case 8:
                SelectLanguage(&LanguageCode);
                break;
            default:
                printf("Invalid selection.Please try again:\n");
                continue;
            }
            break;
        }
    }
    return;
}

int UserSelectionJudgment()
{
    char Choice;
    for (;;)
    {
        getchar();
        scanf("%c", &Choice);
        if (Choice == 'y' || Choice == 'Y')
            return 1;
        else if (Choice == 'n' || Choice == 'N')
            return 0;
        else
            printf("Invalid selection.Please try again:\n");
        continue;
    }
}

long CollectStudentIDAndScores(long StudentIDAndScores[], int *StudentNumber)
{
    int Count = 0;
    long StudentID, Scores;
    Count = 0;
    printf("Please enter the number of the students:");
    scanf("%d", &*StudentNumber);
    printf("Please enter the student ID and scores in the form of \"ID scores\".\n");
    while (scanf("%ld %ld", &StudentID, &Scores) != EOF)
    {
        if (StudentID <= 0 || Scores < 0)
        {
            printf("Negative numbers cannot be entered!Please try again!\n");
            continue;
        }
        else
        {
            if (Count == *StudentNumber * 2)
            {
                printf("Data input succeeded!\nThe number of students:%d\n", *StudentNumber);
                break;
            }
            else
            {
                StudentIDAndScores[Count++] = StudentID;
                StudentIDAndScores[Count++] = Scores;
                printf("Success! ID:%ld Scores:%ld\n", StudentIDAndScores[Count - 2], StudentIDAndScores[Count - 1]);
            }
        }
    }
    return StudentIDAndScores[MaxData];
}

long CalculateTotalAndAverageScores(long StudentIDAndScores[], int *StudentNumber, long Total[], double Average[])
{
    int StudentNumberCount;
    if (StudentIDAndScores[0] == 0)
    {
        printf("You have not entered relevant data!\n");
        return StudentIDAndScores[MaxData];
    }
    else
    {
        for (StudentNumberCount = *StudentNumber; StudentNumberCount > 0; StudentNumberCount--)
        {
            Total[StudentNumberCount];
        }
    }
    return StudentIDAndScores[MaxData], Total[(MaxData - 1) / 2], Average[(MaxData - 1) / 2];
}

void SelectLanguage(int *LanguageCode)
{
    char UserSelect[5];
    Text(0, LanguageCode);
    printf("chs:Simplified Chinese\ten:English\n");
    for(;;)
    {
        scanf("%s", UserSelect);
        if (stricmp(UserSelect, "en") == 0)
            *LanguageCode = 0;
        else if (stricmp(UserSelect, "chs") == 0)
            *LanguageCode = 1;
        else
        {
            Text(2, LanguageCode);
            continue;
        }
        Text(1, LanguageCode);
        return;
    }
}

void Text(int TextCode, int *LanguageCode)
{
    switch(TextCode + *LanguageCode * 1000)
    {
    case 0: printf("Please select your language:\n"); return;
    case 1: printf("Language changed successfully!\n"); return;
    case 2: printf("No corresponding language, Please select again!\n"); return;
    case 3: printf("1.Input record\n"
                   "2.Calculate total and average score of course\n"
                   "3.Sort in descending order by number\n"
                   "4.Sort in ascending order by number\n"
                   "5.Search by number\n"
                   "6.Statistic analysis\n"
                   "7.List record\n"
                   "8.Change language\n"
                   "0.Exit\n"
                   "Please enter your choice:\n"); return;
    case 4: printf("\n"); return;
    case 5: printf("\n"); return;
    case 6: printf("\n"); return;
    case 7: printf("\n"); return;
    case 8: printf("\n"); return;
    case 9: printf("\n"); return;
    case 10: printf("\n"); return;
    case 11: printf("\n"); return;
    case 12: printf("\n"); return;
    case 13: printf("\n"); return;
    case 1000: printf("��ѡ��������ԣ�\n"); return;
    case 1001: printf("���Ը��ĳɹ���\n"); return;
    case 1002: printf("û�ж�Ӧ�����ԣ�������ѡ��һ�Σ�\n"); return;
    case 1003: printf("1.¼��ÿ��ѧ����ѧ�źͿ��Գɼ�\n"
                      "2.����γ̵��ֺܷ�ƽ����\n"
                      "3.���ɼ��ɸߵ����ų����α�\n"
                      "4.��ѧ����С�����ų��ɼ���\n"
                      "5.��ѧ�Ų�ѯѧ���������俼�Գɼ�\n"
                      "6.�����㣨90~100�������ã�80~89�����еȣ�70~79��������60~69����������0~59��5�����ͳ��ÿ�����������Լ���ռ�İٷֱ�\n"
                      "7.���ÿ��ѧ����ѧ�š����Գɼ�\n"
                      "8.��������\n"
                      "0.�˳�����\n"
                      "���������:\n"); return;
    }
}