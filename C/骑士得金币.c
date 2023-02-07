#include <stdio.h>
int main()
{
    int k = 0;
    int i = 0;
    int cnt_0 = 1, cnt_1 = 0;
    int gold = 0;
    scanf("%d", &k);
    for (; i < k;)
    {
        for (cnt_1 = 1; cnt_1 <= cnt_0; cnt_0++)
        {
            gold += cnt_0;
            cnt_1++;
            i++;
            if (i == k)
            {
                break;
            }
        }
    }
    printf("在前%d天，骑士一共获得了%d个金币", k, gold);
    return 0;
}