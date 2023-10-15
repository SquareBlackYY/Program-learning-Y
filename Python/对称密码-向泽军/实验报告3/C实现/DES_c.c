#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// PC-1表，用于密钥置换
int pc1_table[56] = {
    57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4};

// 左移位数表
int loop_table[16] = {
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

// PC-2表，用于生成子密钥
int pc2_table[48] = {
    14, 17, 11, 24, 1, 5,
    3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32};

// 置换IP表
int ip_table[64] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7};

// 扩展运算表
int expansion_table[48] = {
    32, 1, 2, 3, 4, 5,
    4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32, 1};

// S盒
int s_box[8][4][16] = {
    // S1
    {
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},
    // S2
    {
        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
        {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
        {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},
    // S3
    {
        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
        {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
        {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},
    // S4
    {
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
        {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
        {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
        {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},
    // S5
    {
        {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
        {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
        {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
        {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},
    // S6
    {
        {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
        {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
        {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
        {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},
    // S7
    {
        {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
        {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},
    // S8
    {
        {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
        {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
        {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
        {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}};

// P换位表
int p_table[32] = {
    16, 7, 20, 21,
    29, 12, 28, 17,
    1, 15, 23, 26,
    5, 18, 31, 10,
    2, 8, 24, 14,
    32, 27, 3, 9,
    19, 13, 30, 6,
    22, 11, 4, 25};

const int size_of_char = sizeof(char);
const int size_of_char_s = sizeof(char *);

char *hex_to_binary(const char *hex);                                                   // 16进制转2进制
char *binary_to_hex(const char *binary);                                                // 2进制转16进制
void DES(const char *text, const char *seed_key, const int mode, char *text_processed); // DES函数
char **generate_key_schedule(const char *seed_key);                                     // 密钥生成函数
char *DES_initial_permutation(const char *bits, const int mode);                        // IP置换函数
char *DES_round(char *bits, const char *key);                                           // 轮函数
char *DES_expand(const char *bits);                                                     // 扩展运算函数
char *DES_compress(const char *bits);                                                   // 压缩运算函数(S盒)
char *DES_permute(const char *bits);                                                    // P置换
void exchange_strings(char *str);                                                       // 字符串左右两半交换函数
char *xor_strings(const char *str1, const char *str2);                                  // 异或函数

int main()
{
    const char *seed_key = "1f1f1f1f0e0e0e0e";
    char *key_binary = hex_to_binary(seed_key);
    const char *plain_text_hex = "0123456789abcdef";
    char *plain_text_binary = hex_to_binary(plain_text_hex);

    char *text_processed_binary = (char *)malloc(size_of_char * strlen(plain_text_binary) + 1);
    DES(plain_text_binary, key_binary, 0, text_processed_binary);
    char *ciper_text_hex = binary_to_hex(text_processed_binary);
    printf("加密结果:%s\n", ciper_text_hex);

    char *ciper_text_binary = hex_to_binary(ciper_text_hex);
    DES(ciper_text_binary, key_binary, 1, text_processed_binary);
    char *new_plain_text_hex = binary_to_hex(text_processed_binary);
    printf("解密结果:%s\n", new_plain_text_hex);

    free(new_plain_text_hex);
    free(ciper_text_binary);
    free(ciper_text_hex);
    free(text_processed_binary);
    free(plain_text_binary);
    free(key_binary);
    return 0;
}

char *hex_to_binary(const char *hex)
{
    int hex_length = strlen(hex), binary_length = hex_length * 4;
    char *binary = (char *)malloc(size_of_char * hex_length * 4 + 1);
    for (int i = 0; i < hex_length; i++)
    {
        char c = hex[i];
        int value = strtoul(&c, NULL, 16);
        for (int j = 3; j >= 0; j--)
            binary[i * 4 + j] = '0' + ((value >> (3 - j)) & 1);
    }
    binary[hex_length * 4] = '\0';
    return binary;
}

char *binary_to_hex(const char *binary)
{
    int binary_length = strlen(binary), hex_length = (binary_length + 3) / 4;
    char *hex = (char *)malloc(size_of_char * hex_length + 1);
    for (int i = 0; i < hex_length; i++)
    {
        int value = 0;
        for (int j = 0; j < 4; j++)
            if (i * 4 + j < binary_length)
                value = (value << 1) + (binary[i * 4 + j] - '0');
        sprintf(&hex[i], "%x", value);
    }
    hex[hex_length] = '\0';
    return hex;
}

void DES(const char *text, const char *seed_key, const int mode, char *text_processed)
{
    char **key_schedule = generate_key_schedule(seed_key);
    char *text_groups = (char *)malloc(size_of_char * 64 + 1);
    char *text_rounded = (char *)malloc(size_of_char * 64 + 1);
    int number_of_text_groups = strlen(text) / 64;
    for (int i = 0; i < number_of_text_groups; i++)
    {
        memcpy(text_groups, text + i * 64, 64);
        text_groups[64] = '\0';
        char *bits_iped = DES_initial_permutation(text_groups, 0);
        memcpy(text_rounded, bits_iped, 64);
        text_rounded[64] = '\0';
        for (int round_num = 0; round_num < 16; round_num++)
            if (mode == 0)
                text_rounded = DES_round(text_rounded, key_schedule[round_num]);
            else
                text_rounded = DES_round(text_rounded, key_schedule[15 - round_num]);
        exchange_strings(text_rounded);
        char *bits_ip_1ed = DES_initial_permutation(text_rounded, 1);
        memcpy(text_processed + i * 64, bits_ip_1ed, 64);
        free(bits_ip_1ed);
        free(bits_iped);
    }
    text_processed[strlen(text)] = '\0';
    free(text_rounded);
    free(text_groups);
    for (int i = 0; i < 16; i++)
        free(key_schedule[i]);
    free(key_schedule);
}

char **generate_key_schedule(const char *seed_key)
{
    char permuted_key[56 + 1], left[28 + 1], right[28 + 1];
    for (int i = 0; i < 56; i++)
        permuted_key[i] = seed_key[pc1_table[i] - 1];
    permuted_key[56] = '\0';
    memcpy(left, permuted_key, 28);
    left[28] = '\0';
    memcpy(right, permuted_key + 28, 28);
    right[28] = '\0';
    char **key_schedule = (char **)malloc(size_of_char_s * 16);
    for (int i = 0; i < 16; i++)
        key_schedule[i] = (char *)malloc(size_of_char * 48 + 1);
    for (int round_num = 0; round_num < 16; round_num++)
    {
        char combined_key[56];
        for (int i = 0; i < 28; i++)
        {
            left[i] = left[(i + loop_table[round_num]) % (28)];
            right[i] = right[(i + loop_table[round_num]) % (28)];
        }
        memcpy(combined_key, left, 28);
        memcpy(combined_key + 28, right, 28);
        for (int i = 0; i < 48; i++)
            key_schedule[round_num][i] = combined_key[pc2_table[i] - 1];
        key_schedule[round_num][48] = '\0';
    }
    return key_schedule;
}

char *DES_initial_permutation(const char *bits, const int mode)
{
    char *bits_iped = (char *)malloc(size_of_char * 64 + 1);
    if (mode == 0)
        for (int i = 0; i < 64; i++)
            bits_iped[i] = bits[ip_table[i] - 1];
    else
    {
        for (int i = 0; i < 64; i++)
            bits_iped[ip_table[i] - 1] = bits[i];
    }
    bits_iped[64] = '\0';
    return bits_iped;
}

char *DES_round(char *bits, const char *key)
{
    char left[32 + 1], right[32 + 1];
    memcpy(left, bits, 32);
    left[32] = '\0';
    memcpy(right, bits + 32, 32);
    right[32] = '\0';
    char *right_expanded = DES_expand(right);
    char *right_xored_with_key = xor_strings(right_expanded, key);
    char *bits_compressed = DES_compress(right_xored_with_key);
    char *bits_permuted = DES_permute(bits_compressed);
    char *right_new = xor_strings(left, bits_permuted);
    memcpy(bits, right, 32);
    memcpy(bits + 32, right_new, 32);
    bits[64] = '\0';
    free(right_expanded);
    free(right_xored_with_key);
    free(bits_compressed);
    free(bits_permuted);
    free(right_new);
    return bits;
}

char *DES_expand(const char *bits)
{
    char *expanded_bits = (char *)malloc(size_of_char * 48 + 1);
    for (int i = 0; i < 48; i++)
        expanded_bits[i] = bits[expansion_table[i] - 1];
    expanded_bits[48] = '\0';
    return expanded_bits;
}

char *DES_compress(const char *bits)
{
    char *compressed_bits = (char *)malloc(size_of_char * 32 + 1);
    for (int i = 0; i < 48; i += 6)
    {
        int row = (bits[i] - '0') * 2 + (bits[i + 5] - '0');
        int col = (bits[i + 1] - '0') * 8 + (bits[i + 2] - '0') * 4 + (bits[i + 3] - '0') * 2 + (bits[i + 4] - '0');
        int value = s_box[i / 6][row][col];
        compressed_bits[(i / 6) * 4 + 0] = (value & 0x8) ? '1' : '0';
        compressed_bits[(i / 6) * 4 + 1] = (value & 0x4) ? '1' : '0';
        compressed_bits[(i / 6) * 4 + 2] = (value & 0x2) ? '1' : '0';
        compressed_bits[(i / 6) * 4 + 3] = (value & 0x1) ? '1' : '0';
    }
    compressed_bits[32] = '\0';
    return compressed_bits;
}

char *DES_permute(const char *bits)
{
    char *permuted_bits = malloc(size_of_char * 33 + 1);
    for (int i = 0; i < 32; i++)
        permuted_bits[i] = bits[p_table[i] - 1];
    permuted_bits[32] = '\0';
    return permuted_bits;
}

void exchange_strings(char *str)
{
    int length = strlen(str);
    char *start = str, *end = str + length / 2;
    while (start < str + length && end < str + length)
    {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end++;
    }
}

char *xor_strings(const char *str1, const char *str2)
{
    int length = strlen(str1);
    char *result = (char *)malloc(size_of_char * length + 1);
    for (int i = 0; i < length; i++)
        result[i] = (str1[i] ^ str2[i]) + '0';
    result[length] = '\0';
    return result;
}