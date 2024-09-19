// ch8_17.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BIT_NUM 50

void printBigInt(int *bit)
{
    int i;
    for (i = BIT_NUM - 1; i >= 0; i--)
        if (bit[i] != 0)
            break;
    for (; i >= 0; i--)
        printf("%d", bit[i]);
    printf("\n");
}

void multiply(int n, int *bit)
{
    int i, j, temp, carry = 0;
    for (i = 0; i < BIT_NUM; i++)
    {
        temp = bit[i] * n + carry;
        carry = temp / 10;
        bit[i] = temp % 10;
    }
}

void printFactorial(int n)
{
    int *bit = (int *)malloc(BIT_NUM * sizeof(int)), i;
    memset(bit, 0, BIT_NUM * sizeof(int));
    bit[0] = 1;
    printf("1! = ");
    printBigInt(bit);
    for (i = 2; i <= n; i++)
    {
        multiply(i, bit);
        printf("%d! = ", i);
        printBigInt(bit);
    }
    free(bit);
}

void ch8_17()
{
    printFactorial(40);
}