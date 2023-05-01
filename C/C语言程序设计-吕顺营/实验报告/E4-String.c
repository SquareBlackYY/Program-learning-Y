#include <stdio.h>

int strLen(char[]);
int strCpy(char[], char[]);
int strCat(char[], char[]);
int strCmp(char[], char[]);

main()
{
	char str1[100], str2[100], str3[100];
	printf("字符串1：");
	gets(str1);
	printf("字符串2：");
	gets(str2);
	printf("1.字符串1的长度为：%d\n", strLen(str1));
	strCpy(str1, str3);
	printf("2.将字符串1复制到字符串3，字符串3：%s\n", str3);
	strCat(str1, str2);
	printf("3.将字符串2连接到字符串1中，字符串1：%s\n", str1);
	printf("4.字符串1与字符串2比较结果为：%d\n", strCmp(str1, str2));
	return 0;
}

int strLen(char str1[])
{
	int i = 0;
	while (str1[i++] != '\0')
		;
	return i - 1;
}

int strCpy(char str1[], char str3[])
{
	while (*str1 != '\0')
	{
		*str3 = *str1;
		str1++;
		str3++;
	}
	*str3 = '\0';
}
int strCat(char str1[], char str2[])
{
	int i = strLen(str1), j = 0;
	while (j != strLen(str2))
	{
		str1[i] = str2[j];
		i++;
		j++;
	}
	str1[i] = '\0';
}
int strCmp(char str1[], char str2[])
{
	while (*str1 && *str2 && *str1 == *str2)
	{
		str1++;
		str2++;
	}
	return *str1 - *str2;
}