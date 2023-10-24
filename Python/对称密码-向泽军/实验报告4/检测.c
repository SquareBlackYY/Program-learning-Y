#include <stdio.h>
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