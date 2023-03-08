#include <stdio.h>
int main()
{
    int Input, NumOfFactor = 1, i, Factor[10000], Temp, PrimeCode = 1;
    scanf("%d", &Input);
    for(Temp = Input;PrimeCode != 0;)
        for(i = 2, PrimeCode = 0;PrimeCode == 0 && i < Temp;i++)
            if(Temp % i == 0)
            {
                Factor[NumOfFactor++ - 1] = i;
                Temp /= i;
                PrimeCode++;
            }
    Factor[NumOfFactor - 1] = Temp; 

    
    printf("%d\n", NumOfFactor);
    for(i = 0;i < NumOfFactor;i++)
    {
        if(i == 0)
            printf("%d", Factor[i]);
        else
            printf("*%d", Factor[i]);
    }
    printf("\n");
    return 0;
}