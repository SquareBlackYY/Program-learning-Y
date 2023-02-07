#include <stdio.h>
#define MaxData 60 //������󳤶�

void CollectStudentIDAndScores(long StudentIDAndScores[], int *StudentNumber);
//��1��	¼��ÿ��ѧ����ѧ�źͿ��Գɼ�
void CalculateTotalAndAverageScores(long StudentIDAndScores[], int *StudentNumber, long *Total, double *Average);
//��2��	����γ̵��ֺܷ�ƽ����
void SortByGrade(long StudentIDAndScores[], int *StudentNumber);
//��3��	���ɼ��ɸߵ����ų����α�
void SortByID(long StudentIDAndScores[], int *StudentNumber);
//��4��	��ѧ����С�����ų��ɼ���
void QueryRankAndScore(long StudentIDAndScores[], int *StudentNumber);
//��5��	��ѧ�Ų�ѯѧ���������俼�Գɼ�
void StatisticsAndClassification(long StudentIDAndScores[], int *StudentNumber);
//��6��	�����㣨90~100�������ã�80~89�����еȣ�70~79��������60~69����������0~59��5�����ͳ��ÿ�����������Լ���ռ�İٷֱ�
void OutputIDAndScore(long StudentIDAndScores[], int *StudentNumber);
//��7��	���ÿ��ѧ����ѧ�š����Գɼ�

void main()
{
    int Choice = -1, Count, StudentNumber;
    long StudentIDAndScores[MaxData] = {0}, Total;
    double Average;
    for (; Choice != 0;)
    {
        printf("1.Input record\n"
               "2.Calculate total and average score of course\n"
               "3.Sort in descending order by number\n"
               "4.Sort in ascending order by number\n"
               "5.Search by number\n"
               "6.Statistic analysis\n"
               "7.List record\n"
               "0.Exit\n"
               "Please enter your choice:");
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
                CalculateTotalAndAverageScores(StudentIDAndScores, &StudentNumber, &Total, &Average);
                break;
            case 3:
                SortByGrade(StudentIDAndScores, &StudentNumber);
                break;
            case 4:
                SortByID(StudentIDAndScores, &StudentNumber);
                break;
            case 5:
                QueryRankAndScore(StudentIDAndScores, &StudentNumber);
                break;
            case 6:
                StatisticsAndClassification(StudentIDAndScores, &StudentNumber);
                break;
            case 7:
                OutputIDAndScore(StudentIDAndScores, &StudentNumber);
                break;
            default:
                printf("Invalid selection.Please try again:");
                continue;
            }
            break;
        }
    }
    return;
}

//��1��	¼��ÿ��ѧ����ѧ�źͿ��Գɼ�
void CollectStudentIDAndScores(long StudentIDAndScores[], int *StudentNumber)
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
            StudentIDAndScores[Count++] = StudentID;
            StudentIDAndScores[Count++] = Scores;
            printf("Success! ID:%ld Scores:%ld\n", StudentIDAndScores[Count - 2], StudentIDAndScores[Count - 1]);
            if (Count == *StudentNumber * 2)
            {
                printf("Data input succeeded!\n");
                break;
            }
        }
    }
    return;
}

//��2��	����γ̵��ֺܷ�ƽ����
void CalculateTotalAndAverageScores(long StudentIDAndScores[], int *StudentNumber, long *Total, double *Average)
{
    int StudentNumberCount;
    if (StudentIDAndScores[0] == 0)
    {
        printf("You have not entered relevant data!\n");
        return;
    }
    else
    {
        for (StudentNumberCount = *StudentNumber; StudentNumberCount > 0; StudentNumberCount--)
            *Total += StudentIDAndScores[(StudentNumberCount * 2) - 1];
        *Average = *Total * 1.0 / *StudentNumber;
        printf("Total:%d\nAverage:%.2f\n", *Total, *Average);
    }
    return;
}

//��3��	���ɼ��ɸߵ����ų����α�
void SortByGrade(long StudentIDAndScores[], int *StudentNumber)
{
    long Temp[MaxData], TempID, TempScore;
    int Count, InvertedSequence;
    if (StudentIDAndScores[0] == 0)
        printf("You have not entered relevant data!\n");
    else
    {
        for (Count = 0; Count < MaxData; Count++)
            Temp[Count] = StudentIDAndScores[Count];
        for (InvertedSequence = 1; InvertedSequence != 0;)
        {
            for (Count = 0, InvertedSequence = 0; Count + 1 < *StudentNumber; Count++)
            {
                if (Temp[(Count * 2) + 1] < Temp[(Count * 2) + 3])
                {
                    TempScore = Temp[(Count * 2) + 1];
                    TempID = Temp[Count * 2];
                    Temp[(Count * 2) + 1] = Temp[(Count * 2) + 3];
                    Temp[Count * 2] = Temp[(Count * 2) + 2];
                    Temp[(Count * 2) + 3] = TempScore;
                    Temp[(Count * 2) + 2] = TempID;
                    InvertedSequence++;
                }
            }
        }
        printf("Rank\tID\tScore\n");
        for (Count = 0; Count < *StudentNumber; Count++)
            printf("%d\t%ld\t%ld\n", Count + 1, Temp[Count * 2], Temp[(Count * 2) + 1]);
    }
    return;
}

//��4��	��ѧ����С�����ų��ɼ���
void SortByID(long StudentIDAndScores[], int *StudentNumber)
{
    long Temp[MaxData], TempID, TempScore;
    int Count, InvertedSequence;
    if (StudentIDAndScores[0] == 0)
        printf("You have not entered relevant data!\n");
    else
    {
        for (Count = 0; Count < MaxData; Count++)
            Temp[Count] = StudentIDAndScores[Count];
        for (InvertedSequence = 1; InvertedSequence != 0;)
        {
            for (Count = 0, InvertedSequence = 0; Count + 1 < *StudentNumber; Count++)
            {
                if (Temp[(Count * 2)] > Temp[(Count * 2) + 2])
                {
                    TempScore = Temp[(Count * 2) + 1];
                    TempID = Temp[Count * 2];
                    Temp[(Count * 2) + 1] = Temp[(Count * 2) + 3];
                    Temp[Count * 2] = Temp[(Count * 2) + 2];
                    Temp[(Count * 2) + 3] = TempScore;
                    Temp[(Count * 2) + 2] = TempID;
                    InvertedSequence++;
                }
            }
        }
        printf("No.\tID\tScore\n");
        for (Count = 0; Count < *StudentNumber; Count++)
            printf("%d\t%ld\t%ld\n", Count + 1, Temp[Count * 2], Temp[(Count * 2) + 1]);
    }
    return;
}

//��5��	��ѧ�Ų�ѯѧ���������俼�Գɼ�
void QueryRankAndScore(long StudentIDAndScores[], int *StudentNumber)
{
    long Temp[MaxData], TempID, TempScore, UserInput;
    int Count, InvertedSequence;
    if (StudentIDAndScores[0] == 0)
        printf("You have not entered relevant data!\n");
    else
    {
        for (Count = 0; Count < MaxData; Count++)
            Temp[Count] = StudentIDAndScores[Count];
        for (InvertedSequence = 1; InvertedSequence != 0;)
        {
            for (Count = 0, InvertedSequence = 0; Count + 1 < *StudentNumber; Count++)
            {
                if (Temp[(Count * 2) + 1] < Temp[(Count * 2) + 3])
                {
                    TempScore = Temp[(Count * 2) + 1];
                    TempID = Temp[Count * 2];
                    Temp[(Count * 2) + 1] = Temp[(Count * 2) + 3];
                    Temp[Count * 2] = Temp[(Count * 2) + 2];
                    Temp[(Count * 2) + 3] = TempScore;
                    Temp[(Count * 2) + 2] = TempID;
                    InvertedSequence++;
                }
            }
        }
        printf("Enter the student ID to query the rank and score, and enter 0 to exit.\n");
        for (;;)
        {
            scanf("%ld", &UserInput);
            for (Count = 0; Count < *StudentNumber; Count++)
            {
                if (UserInput == 0)
                    break;
                else if (UserInput == Temp[Count * 2])
                {
                    printf("ID\tRank\tScore\n%ld", UserInput, Count + 1, Temp[(Count * 2) + 1]);
                    continue;
                }
            }
            printf("No corresponding student ID found, please enter again:\n");
        }
    }
    return;
}

//��6��	�����㣨90~100�������ã�80~89�����еȣ�70~79��������60~69����������0~59��5�����ͳ��ÿ�����������Լ���ռ�İٷֱ�
void StatisticsAndClassification(long StudentIDAndScores[], int *StudentNumber)
{
    int Count, LevelNumber[5] = {0};
    char Level[5][6] = {"����", "����", "�е�", "����", "������"};
    if (StudentIDAndScores[0] == 0)
        printf("You have not entered relevant data!\n");
    else
    {
        for (Count = 0; Count < *StudentNumber; Count++)
        {
            if (StudentIDAndScores[(Count * 2) + 1] >= 90 && StudentIDAndScores[(Count * 2) + 1] <= 100)
                LevelNumber[0]++;
            else if (StudentIDAndScores[(Count * 2) + 1] >= 80)
                LevelNumber[1]++;
            else if (StudentIDAndScores[(Count * 2) + 1] >= 70)
                LevelNumber[2]++;
            else if (StudentIDAndScores[(Count * 2) + 1] >= 60)
                LevelNumber[3]++;
            else
                LevelNumber[4]++;
        }
        for (Count = 0; Count <= 4; Count++)
            printf("%s\t%ld\t%.2f%%\n", Level[Count], LevelNumber[Count], LevelNumber[Count] * 100.0 / *StudentNumber);
    }
    return;
}

//��7��	���ÿ��ѧ����ѧ�š����Գɼ�
void OutputIDAndScore(long StudentIDAndScores[], int *StudentNumber)
{
    int Count;
    if (StudentIDAndScores[0] == 0)
        printf("You have not entered relevant data!\n");
    else
    {
        printf("ID\tScore\n");
        for (Count = 0; Count < *StudentNumber; Count++)
            printf("%ld\t%ld\n", StudentIDAndScores[Count * 2], StudentIDAndScores[(Count * 2) + 1]);
    }
    return;
}