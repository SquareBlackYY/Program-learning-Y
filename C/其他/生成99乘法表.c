#include <stdio.h>
int main()
{
	int a = 1, b = 1,cnt=1;
	for (; b < 10; b++)
	{
		for (; a <= b; a++)
		{
			printf("%d*%d=%d", a, b, a * b);
			if (a == b)
				printf("\n");
			else
				printf(" ");
		}
		a = cnt;
		cnt + 1;
	}
	printf("end");
}
