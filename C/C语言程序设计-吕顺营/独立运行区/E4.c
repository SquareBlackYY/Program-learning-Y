#include <stdio.h>

int strLen(char str[]);
void strCpy(char src[], char dst[]);
void strCat(char str1[], char str2[]);
int strCmp(char str1[], char str2[]);


int main()
{
    char str1[100] = "Hello", str2[20] = "World";
    printf("Original string: %s\n", str1);

    // 测试字符串长度函数
    printf("The length of str1 is %d\n", strLen(str1));

    // 测试字符串复制函数
    strCpy(str1, str2);
    printf("After copying, str2 is %s\n", str2);

    // 测试字符串连接函数
    strCat(str1, " World!");
    printf("After concatenation, str1 is %s\n", str1);

    // 测试字符串比较函数
    char str3[20] = "Boy";
    char str4[20] = "Boy";
    printf("str3 and str4 compare：%d\n", strCmp(str3, str4)); // 输出0
    char str5[20] = "Boy";
    char str6[20] = "Girl";
    printf("str5 and str6 compare：%d\n", strCmp(str5, str6)); // 输出小于0的值
    char str7[20] = "Cats";
    char str8[20] = "Dogs";
    printf("str7 and str8 compare：%d\n", strCmp(str7, str8)); // 输出大于0的值

    return 0;
}

int strLen(char str[]) // 求字符串长度
{
    int i = 0;
    while (str[i] != '\0')
    {
        i++;
    }
    return i;
}

void strCpy(char src[], char dst[]) // 字符串复制
{
    int i = 0;
    while (src[i] != '\0')
    {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

void strCat(char str1[], char str2[]) // 字符串连接
{
    int i = 0;
    while (str1[i] != '\0')
    {
        i++;
    }
    int j = 0;
    while (str2[j] != '\0')
    {
        str1[i] = str2[j];
        i++;
        j++;
    }
    str1[i] = '\0';
}

int strCmp(char str1[], char str2[]) // 字符串比较
{
    int i = 0;
    while (str1[i] == str2[i] && str1[i] != '\0' && str2[i] != '\0')
    {
        i++;
    }
    return str1[i] - str2[i];
}