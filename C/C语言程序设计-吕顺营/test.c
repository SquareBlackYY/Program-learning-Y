#include <stdio.h>
void swap(int x, int y);
int main()
{
	int a, b;
	a = 5;
	b= 6;
	printf("a=%d b=%d\n", a, b);
	swap(a, b);
	printf("a=%d b=%d\n", a, b);
}
void swap(int x, int y)
{
	int t;
	t = x;
	x = y;
	y = t;
}