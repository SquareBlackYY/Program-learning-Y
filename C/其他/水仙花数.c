#include <stdio.h>

int main()
{
	int input;//ˮ�ɻ���
	scanf("%d", &input);
	int cweishu = input;
	int start = 1;
	int i = 0;
	int mi = 1;
	int cmi = input;
	int cand = 0;
	int cjieguo = 0;
	for (; cweishu > 1; cweishu--)
	{
		start *= 10;
	}
	int output = start;
	int count = start*10;
	for (; start < count;)
	{
		cweishu = input;
		for (; cweishu > 0; cweishu--)
		{
			i = start % 10;
			cmi = input;
			mi = 1;
			for (; cmi > 0; cmi--)
			{
				mi *= i;
			}
			cand += mi;
			start /= 10;
		}
		if (cand == output)
		{
			cjieguo++;
			printf("��%d������ǣ�%d\n", cjieguo, output);
		}
		start = output + 1;
		output = start;
		cand = 0;
	}
	printf("���н���");
	return 0;
}