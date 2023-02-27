#include<stdio.h>
int main()
{
  double pai=0;
    int start=1,a=1;
    for(;(double)1/(start+2)>=0.0001;start+=2,a*=-1)
        pai+=(double)1/start*a;
    printf(".4f",pai*4);
}
if(term<0)
    term*=-1