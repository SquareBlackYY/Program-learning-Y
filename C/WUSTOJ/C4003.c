#include <stdio.h>
int main()
{
    int n;
    scanf("%d", &n);
    if ((n % 4 == 0 && n % 100 != 0) || n % 400 == 0)
        printf("%d is a leap year!", n);
    else
        printf("%d isn't a leap year!", n);
    return 0;
}