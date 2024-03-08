#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main()
{
    int n, i, k, cnt;
    int *p;
    char *t;
    t = (char *)malloc(sizeof(char) * 100);
    p = (int *)malloc(sizeof(int) * 100);
    while (scanf("%d", &n) != EOF)
    {
        for (i = 0; i < n; i++)
        {
            scanf("%s", t);
            cnt = 0;
            for (k = 0; k < strlen(t); k++)
                if (t[k] >= '0' && t[k] <= '9')
                    cnt++;
            p[i] = cnt;
        }
        for (i = 0; i < n; i++)
            printf("%d\n", p[i]);
    }
    return 0;
}