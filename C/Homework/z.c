#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
int main()
{
    int a, b, i, x, sum;
    int j = 0;
    int cnt = 0;
    int mode = 0;
    printf("当回答除法时，保留两位小数\n");
    while (j == 0)
    {
        for (i = 0; i < 10; i++)
        {
            srand(time(0));
            a = rand() % 10 + 1;
            b = rand() % 10 + 1;
            mode = rand() % 4;
            switch (mode)
            {
            case 0:
                printf("%d+%d = \n", a, b);
                sum = a + b;
                break;
            case 1:
                printf("%d-%d = \n", a, b);
                sum = a - b;
                break;
            case 2:
                printf("%d*%d = \n", a, b);
                sum = a * b;
                break;
            case 3:
                printf("%d/%d = \n", a, b);
                sum = a / b;
                break;
            default:
                printf("somethingis wrong!\n");
                break;
            }
            scanf("%d", &x);
            if (fabs(sum - x) - 0.05 < 0.01 && sum - x != 0.05)
            {

                mode = rand() % 4;
                switch (mode)
                {
                case 0:
                    printf("Very good!\n");
                    break;
                case 1:
                    printf("Excellent!\n");
                    break;
                case 2:
                    printf("Nice work!\n");
                    break;
                case 3:
                    printf("Keep up the good word\n");
                    break;
                    cnt++;
                }
            }
            else
            {
                mode = rand() % 4;
                switch (mode)
                {
                case 0:
                    printf("No.Please try again.\n");
                    break;
                case 1:
                    printf("Wrong.Try once more.\n");
                    break;
                case 2:
                    printf("Dont't give up!\n");
                    break;
                case 3:
                    printf("not correct.keep trying\n");
                    break;
                }
            }
        }
        if (cnt > 7)
        {
            j = 1;
        }
        else
        {
            printf("再试一次：\n");
        }
    }
    printf("正确率为%d%%\n", cnt * 10);
    printf("得分为%d", cnt * 10);
    return 0;
}