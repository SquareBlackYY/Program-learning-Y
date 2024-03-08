// ch6_9.c

#include <stdio.h>

void ch6_9()
{
    int n = 0, i, term;
    double e = 1;

    for (n = 1;; n++)
    {
        for (term = 1, i = 1; i <= n; i++)
            term *= i;
        if (1.0 / term < 0.00001)
            break;
        e += 1.0 / term;
    }

    printf("e=%.5f,累加项数：%d\n", e, n + 1);
}