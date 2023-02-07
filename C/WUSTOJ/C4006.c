#include <stdio.h>
int main()
{
    int n;
    scanf("%d", &n);
    if (n > 85)
        printf("very good");
    else if (n >= 60)
        printf("good");
    else
        printf("no good");
    return 0;
}