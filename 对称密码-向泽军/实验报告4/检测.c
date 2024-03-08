#include <stdio.h>
#include <stdint.h>
int main()
{
    // 密钥表检测
    for (int i = 0; i < 11; i++)
    {
        for (int j = 0; j < 16; j++)
            printf("%3d ", key_schedule[i][j]);
        printf("\n");
    }

    // 文本检测
    for (int i = 0; i < 11; i++)
        printf("%3d ", text[i]);
    printf("\n");

    
    return 0
}

uint8_t *ta(uint8_t *array)
{
    uint8_t tda[16];
    printf("转置前: ");
    for (int i = 0; i < 16; i++)
        printf("%3d ", array[i]);
    printf("\n转置后: ");
    for (int i = 0; i < 16; i++)
    {
        tda[i] = array[TRANSPOSE_TABLE[i]];
        printf("%3d ", tda[i]);
    }
    printf("\n");
    exit(0);
}