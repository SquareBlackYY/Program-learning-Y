#include <stdio.h>
int main()
{
    int a[1000], cnt, i, j, temp;
    printf("请输入不超过1000个整数：");
    printf("\n你想输入几个整数？");
    scanf("%d", &cnt);
    for (i = 0; i < cnt; i++)
        scanf("%d", &a[i]);
    for (i = 1; i < cnt; i++)
        for (j = 0; j < cnt - i; j++)
            if (a[j] < a[j + 1])
            {
                temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }
    for (i = 0; i < cnt; i++)
        printf("%d", a[i]);
    return 0;
}