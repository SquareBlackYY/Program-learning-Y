#include <stdio.h>
int main()
{
    int n, m, cntn, cntm, a[50][5], n0[50], m0[5], mid, i, cnt, k;
    double aven[50], avem[5];
    while (scanf("%d %d", &n, &m) != EOF)
    {
        for (cntn = 0; cntn < n; cntn++)
            n0[cntn] = 0;
        for (cntm = 0; cntm < m; cntm++)
            m0[cntm] = 0;
        for (cntn = 0; cntn < n; cntn++)
        {
            for (cntm = 0; cntm < m; cntm++)
            {
                scanf("%d", &mid);
                a[cntn][cntm] = mid;
                n0[cntn] += mid;
                m0[cntm] += mid;
            }
        }
        for (i = 0; i < n; i++)
            aven[i] = (double)n0[i] / m;
        for (i = 0; i < m; i++)
            avem[i] = (double)m0[i] / n;
        for (cntn = 0, cnt = 0; cntn < n; cntn++)
        {
            for (cntm = 0, k = 0; cntm < m; cntm++)
            {
                if (a[cntn][cntm] < avem[cntm])
                    break;
                else
                    k++;
            }
            if (k == m)
                cnt++;
        }
        for (cntn = 0; cntn < n; cntn++)
        {
            if (cntn != 0)
                printf(" ");
            printf("%.2f", aven[cntn]);
        }
        printf("\n");
        for (cntm = 0; cntm < m; cntm++)
        {
            if (cntm != 0)
                printf(" ");
            printf("%.2f", avem[cntm]);
        }
        printf("\n%d\n\n", cnt);
    }
    return 0;
}