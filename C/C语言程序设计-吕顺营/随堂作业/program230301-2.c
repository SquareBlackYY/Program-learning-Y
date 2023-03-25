#include<stdio.h>
int main()
{
    int n, i, j, k, m = 0;
    printf("1000以内素数如下：");
    for(n = 2;n <= 1000;n++)
    {
        for(i = 2, j = 0;i < n && j == 0;i++)
            if(n % i == 0) 
                j++;
        if(j == 0)
        {
            if(m % 5 == 0)
                printf("\n");
            printf("%d\t", n);
            m++;
        }
    }
}