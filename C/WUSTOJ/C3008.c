#include <stdio.h>
int main()
{
    double ck, ll;
    scanf("%lf %lf", &ck, &ll);
    printf("%11.2f\n%11.2f\n%11.2f", ck, ck * ll * 0.01, ck + ck * ll * 0.01);
    return 0;
}