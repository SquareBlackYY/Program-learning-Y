#include <stdio.h>
int main()
{
    int input, output = 0;
    for(;;)
    {
        scanf("%d", &input);
        if(input <= 0) break;
        output += input;
    }
    printf("%d\n", output);
}