#include <stdio.h>
#include <math.h>
int main() 
{
 double e, sum = 0, term, sign = 1;
 int count = 0, n = 1, i;
 for(;;)
 {
    for(term = 1, i = 1;i <= n;i++)
        term = term * i;

    if(fabs(1 / term) < 0.00001) break;

    sum = sum + 1 / term;
    count++;
    n = n + 1;
 }
 e = sum + 1;
 printf("e=%f\ncount=%d\n", e, count + 1);
}