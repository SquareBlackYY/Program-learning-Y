#include <stdio.h>
int main()
{
    int Year, Month, i, j, Week;
    char Choice;
    for(;;)
    {
        int NumOfDay[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        printf("请输入一个年份，打印该年年历：");
        scanf("%d", &Year);
        if((Year & 4 == 0 && Year % 100 != 0) || Year % 400 == 0)
            NumOfDay[1] = 29;
        Week = (((Year - 1) % 100) + ((Year - 1) % 100) / 4 + ((Year - 1) / 100) / 4 - 2 * ((Year - 1) / 100) + 36) % 7;
        printf("=====%d年 年历=====\n", Year);
        for(Month = 1, j = 0;Month <= 12;Month++)
        {
            printf("--------%2d 月--------\n", Month);
            printf(" 日 一 二 三 四 五 六\n", Month);
            for(i = 1 - Week;Month == 1 && i != 1;i++, j++)
                printf("   ");
            for(i = 1, i = i - (j % 7);i <= NumOfDay[Month - 1];i++)
            {
                if(i >= 1 && i <=NumOfDay[Month - 1])
                {
                    printf(" %2d", i);
                    j++;
                }
                else
                    printf("   ");
                if(j % 7 == 0 || i == NumOfDay[Month - 1])
                    printf("\n");
            }
        }
        for(;;)
        {
            printf("继续使用？(y/n)");
            getchar();
            scanf("%c", &Choice);
            if(Choice == 'y' || Choice == 'Y' || Choice == 'n' || Choice == 'N')
                break;
        }
        if(Choice == 'n' || Choice == 'N')
            break;
    }
    return 0;
}