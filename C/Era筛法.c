#include<stdio.h>
#include<math.h>
#define MAX 32767
int main()
{
    int Input, Range, List[MAX], n1, n2, n3, num, NumberOfPrime;
    printf("求不超过n的所有素数：");
    scanf("%d", &Input);
    Range = pow(Input,0.5);
    for(n1 = 1;n1 <= Input;n1++)//List初始化
        List[n1] = 0;
    for(n1 = 1;n1 <= Range;n1++)
    {
        for(n2 = 2, num = 0;n2 < n1;n2++)
            if(n1%n2 == 0)
                num++;
        if(num != 0)
            List[n1]++;
    }
    for(n1 = 2; n1 <= Range;n1++)
        if(List[n1] == 0)
            for(n2 = n1, n3 = n1;n2 * n3 <= Input;n3++)
                List[n2 * n3]++;
    for(NumberOfPrime = 0, n1 = 2;n1 < Input;n1++)
        if(List[n1] == 0)
            NumberOfPrime++;
    printf("小于%d的素数共有%d个，分别为：", Input, NumberOfPrime);
    for(n1 = 2, num = 0;n1 < Input;n1++)
    {
        if(num == 0 && List[n1] == 0)
        {
            printf("%d", n1);
            num++;
        }
        else if (List[n1] == 0)
            printf(",%d", n1);
    }
    return 0;
}