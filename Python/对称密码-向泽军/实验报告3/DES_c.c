#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void hex_to_binary(char *hex, char *binary);

int main()
{
    char seed_key[16] = "1f1f1f1f0e0e0e0e";
    char *key = seed_key;
    char *p;
    puts(key);
    printf("%ld\n", strlen(key));
    char e = 'e';
    hex_to_binary(&e, p);
    puts(p);
}

void hex_to_binary(char *hex, char *binary)
{
    binary = (char *)malloc(strlen(hex) * 4 * sizeof(char));
    binary[0] = '\0';
    int bit;
    char bit_char[2];
    for (int i = 3; i >= 0; i--)
    {
        bit = (*hex >> i) & 1;
        bit_char[0] = '0' + bit; // 将 bit 转换为字符类型
        bit_char[1] = '\0';
        puts(bit_char);
        strcat(binary, bit_char); // 将字符追加到 binary 中
    }
}
