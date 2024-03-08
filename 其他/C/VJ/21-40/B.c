#include <stdio.h>
int main()
{
    int m, n, cntm, cntn, i, k, max, mid, sm, sn, x, y;
    while (scanf("%d %d", &m, &n) != EOF)
    {
        for (cntm = 1; cntm <= m; cntm++)
        {
            for (cntn = 1; cntn <= n; cntn++)
            {
                scanf("%d", &i);
                if (i < 0)
                {
                    k = -1 * i;
                    x = -1;
                }
                else
                {
                    k = i;
                    x = 1;
                }
                if ((cntm == 1 && cntn == 1) || k > max)
                {
                    max = k;
                    y = x;
                    sm = cntm;
                    sn = cntn;
                }
            }
        }
        printf("%d %d %d\n", sm, sn, max * y);
    }
    return 0;
}