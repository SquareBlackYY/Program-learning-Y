#include<stdio.h>
float f1 (float x);
float f2 (float x);
float Integral (float(*f)(float),float a, float b);


int main()
{
	float y1,y2;
	y1=Integral(f1,0.0,1.0);
	y2=Integral(f2,0.0,3.0);
	printf("函数y1在0到1上的定积分是：%f\n函数y2在0到3上的定积分是：%f\n",y1,y2);
}


float f1(float x)
{
	return x * x + 1;
}


float f2(float x)
{
	return x / ( x * x + 1);
}

float Integral (float(*f)(float),float a, float b)
{
	float x, sum=0;
	for(x = a;x<b;x+=0.001)
	{
		sum+=((*f)(x-0.001)+(*f)(x))*0.001/2;
	}
	return sum;
}