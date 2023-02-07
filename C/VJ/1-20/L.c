#include <stdio.h>

int prime(int input)
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
    int x, y, n, k, i;
    while (scanf("%d %d", &x, &y) != EOF)
    {
        while (x != 0 || y != 0)
        {
            i = y - x + 1;
            for (k = 0; x <= y; x++)
            {
                n = x * x + x + 41;
                if (prime(n) == 1)
                    k++;
            }
            if (k == i)
                printf("OK\n");
            else
                printf("Sorry\n");
            break;
        }
        if (x == 0 && y == 0)
            break;
    }
    return 0;
}