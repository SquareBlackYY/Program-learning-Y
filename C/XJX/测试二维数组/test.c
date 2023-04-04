#include<stdio.h>
int main()
{
    int i, j, a, b, n_1, n_2, n_3;
    printf("您要输入几乘几的矩阵与几乘几的矩阵相乘：");
    scanf("%d%d%d%d",&n_1,&n_2,&n_2,&n_3);
    int element1[n_1][n_2], element2[n_2][n_3];
    printf("请输入第一个矩阵的各个元素：");
    for(i = 0;i<=n_1;i++)
        for(j = 0;j<=n_2;j++)
            scanf("%d",element1[i][j]);
    printf("请输入第二个矩阵的各个元素：");
    for(i = 0;i<=n_1;i++)
        for(j = 0;j<=n_2;j++)
            scanf("%d",element2[i][j]);
    for(a = 0;a<=n_1;a++)
        for(b = 0;(a-1) % n_1 == 0;b++)
    {
        element1[a][b]*element2[b][a];
    }
}