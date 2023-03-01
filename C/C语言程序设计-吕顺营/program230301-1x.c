#include<stdio.h>
int main()
{
	int male, female;
	for(male = 0 ; male <= 45;male++)
		for(female = 0;female <= 45;female++)
				if(4*male+3*female+(45-male-female)/2==98 && (45-male-female) %2==0)
					printf("男人的个数为：%d 女人的个数为：%d 小孩的个数为:%d\n", male, female, 45-male-female);
}