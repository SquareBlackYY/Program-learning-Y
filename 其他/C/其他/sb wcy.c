#include <stdio.h>

int prime(int input) // 返回0不是素数，返回1是素数
{
    int i, output;
    if (input <= 1)
        output = 0;
    else
    {
        for (i = 2, output = 1; i < input; i++)
        {
            if (input % i == 0)
            {
                output = 0;
                break;
            }
        }
    }
    return output;
}

int main()
{
    int i = 2, l = 1;
    for (;i <= 100;)
    {
        if (prime(i) == 1)
        {
            if (l == 1)
            {
                printf("%d", i);
                l++;
            }
            else if (l > 1 && l <=5)
            {
                printf(" %d", i);
                l++;
            }
            else
            {
                printf("\r\n%d", i);
                l = 1;
            }
        }
        i++;
    }
    getchar();
    return 0;
}
