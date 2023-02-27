#include<stdio.h>
int main()
{
    int n, i, term;
    double e = 1;
    for(n = 1;1.0 / term >= 0.00001;n++)
    {
        for(term = 1,i = 1;i <= n;i++)
            term *= i;
        e += 1.0 / term;
    }
    printf("e=%.5f,累加项数：%d\n", e, n + 1);
}