#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// 用于转置的表
const int TRANSPOSE_TABLE[16] = {
    0, 4, 8, 12,
    1, 5, 9, 13,
    2, 6, 10, 14,
    3, 7, 11, 15};

// 适用于字节代换的S盒
const uint8_t S_BOX[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16};

// 适用于逆字节代换的S盒的逆
const uint8_t INV_S_BOX[256] = {
    0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
    0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
    0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
    0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
    0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
    0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
    0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
    0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
    0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
    0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
    0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
    0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
    0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
    0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
    0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D};

// 适用于行移位的映射表
const int SHIFTROWS_TABLE[16] = {
    0, 1, 2, 3,
    5, 6, 7, 4,
    10, 11, 8, 9,
    15, 12, 13, 14};

// 适用于列混淆的矩阵
const uint8_t MIXCOLUMNS_MATRIX[4][4] = {
    {0x02, 0x03, 0x01, 0x01},
    {0x01, 0x02, 0x03, 0x01},
    {0x01, 0x01, 0x02, 0x03},
    {0x03, 0x01, 0x01, 0x02}};

// 适用于逆列混淆的逆矩阵
const uint8_t INV_MIXCOLUMNS_MATRIX[16] = {
    0x0e, 0x0b, 0x0d, 0x09,
    0x09, 0x0e, 0x0b, 0x0d,
    0x0d, 0x09, 0x0e, 0x0b,
    0x0b, 0x0d, 0x09, 0x0e};

// 轮常量
const uint8_t R_CON[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};

void read_input(uint8_t ***, int *);
void write_output(uint8_t **, int);
void generate_key_schedule(char *, uint8_t ***);
void key_extend(uint8_t **);
void RotWord(uint8_t *);
void SubWord(uint8_t *);
void AES_encrypt(int, uint8_t **, uint8_t **);
void AddRoundKey(uint8_t *, uint8_t *);
void subBytes(uint8_t *);
void shiftRows(uint8_t *);
void mixColumns(uint8_t *);

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
}

int main()
{
    uint8_t **text;
    int num_groups;
    read_input(&text, &num_groups);

    uint8_t **key_schedule;
    char *seed_key = "1f1f1f1f0e0e0e0e1f1f1f1f0e0e0e0e";
    generate_key_schedule(seed_key, &key_schedule);

    clock_t start_time, end_time;
    double execution_time;
    // 开始计时
    start_time = clock();

    // 函数执行
    AES_encrypt(num_groups, text, key_schedule);

    // 计时结束
    end_time = clock();
    execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("程序用时: %.2f s\n", execution_time);

    write_output(text, num_groups);

    for (int i = 0; i < 11; i++)
        free(key_schedule[i]);
    free(key_schedule);
    for (int i = 0; i < num_groups; i++)
        free(text[i]);
    free(text);

    return 0;
}

void read_input(uint8_t ***text, int *num_groups)
{
    FILE *input_file = fopen("input.txt", "r");
    fseek(input_file, 0, SEEK_END);
    *num_groups = ftell(input_file) / 32;
    rewind(input_file);

    *text = (uint8_t **)malloc(*num_groups * sizeof(uint8_t *));
    for (int i = 0; i < *num_groups; i++)
        (*text)[i] = (uint8_t *)malloc(16 * sizeof(uint8_t));

    char hex_text[3];
    for (int i = 0; i < *num_groups; i++)
        for (int j = 0; j < 4; j++)
            for (int k = 0; k < 4; k++)
            {
                fread(hex_text, 2, 1, input_file);
                hex_text[2] = '\0';
                (*text)[i][k * 4 + j] = (uint8_t)strtol(hex_text, NULL, 16);
            }

    fclose(input_file);
    printf("文件读取完成。\n");
    printf("文件大小: %.2f MB\n", 16 * (*num_groups) / 1024.0 / 1024);
}

void write_output(uint8_t **text, int num_groups)
{
    FILE *output_file = fopen("output.txt", "w");
    for (int i = 0; i < num_groups; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                uint8_t value = text[i][k * 4 + j];
                uint8_t high_nibble = (value >> 4) & 0xF;
                uint8_t low_nibble = value & 0xF;
                char high_hex = (high_nibble < 10) ? ('0' + high_nibble) : ('A' + high_nibble - 10);
                char low_hex = (low_nibble < 10) ? ('0' + low_nibble) : ('A' + low_nibble - 10);
                fprintf(output_file, "%c%c", high_hex, low_hex);
            }
        }
    }
    fclose(output_file);
    printf("文件写入完成。\n");
}

void generate_key_schedule(char *seed_key, uint8_t ***key_schedule)
{
    char hex_text[3];
    uint8_t temp[16];
    *key_schedule = (uint8_t **)malloc(11 * sizeof(uint8_t *));
    for (int i = 0; i < 11; i++)
        (*key_schedule)[i] = (uint8_t *)malloc(16 * sizeof(uint8_t));

    for (int i = 0; i < 16; i++)
    {
        sscanf(seed_key + (i * 2), "%2s", hex_text);
        (*key_schedule)[0][i] = (uint8_t)strtol(hex_text, NULL, 16);
    }
    key_extend(*key_schedule);

    for (int i = 0; i < 11; i++)
    {
        for (int j = 0; j < 16; j++)
            temp[j] = (*key_schedule)[i][TRANSPOSE_TABLE[j]];
        memcpy((*key_schedule)[i], temp, 16 * sizeof(uint8_t));
    }
    printf("密钥扩展完成。\n");
}

void key_extend(uint8_t **key_schedule)
{
    for (int round_num = 1; round_num < 11; round_num++)
    {
        memcpy(key_schedule[round_num], key_schedule[round_num - 1] + 12, 4 * sizeof(uint8_t));
        RotWord(key_schedule[round_num]);
        SubWord(key_schedule[round_num]);
        key_schedule[round_num][0] ^= key_schedule[round_num - 1][0] ^ R_CON[round_num - 1];
        for (int i = 1; i < 4; i++)
            key_schedule[round_num][i] ^= key_schedule[round_num - 1][i];
        for (int i = 4; i < 16; i++)
            key_schedule[round_num][i] = key_schedule[round_num][i - 4] ^ key_schedule[round_num - 1][i];
    }
}

void RotWord(uint8_t *key_word)
{
    uint8_t temp = key_word[0];
    memcpy(key_word, key_word + 1, 3 * sizeof(uint8_t));
    key_word[3] = temp;
}

void SubWord(uint8_t *key_word)
{
    for (int i = 0; i < 4; i++)
        key_word[i] = S_BOX[key_word[i]];
}

void AES_encrypt(int num_groups, uint8_t **input_text, uint8_t **key_schedule)
{
    uint8_t *text;
    for (int i = 0; i < num_groups; i++)
    {
        text = input_text[i];
        AddRoundKey(text, key_schedule[0]);
        for (int round_num = 1; round_num < 10; round_num++)
        {
            subBytes(text);
            shiftRows(text);
            mixColumns(text);
            AddRoundKey(text, key_schedule[round_num]);
        }
        subBytes(text);
        shiftRows(text);
        AddRoundKey(text, key_schedule[10]);
    }
}

void AddRoundKey(uint8_t *text, uint8_t *key)
{
    for (int i = 0; i < 16; i++)
        text[i] ^= key[i];
}

void subBytes(uint8_t *text)
{
    for (int i = 0; i < 16; i++)
        text[i] = S_BOX[text[i]];
}

void shiftRows(uint8_t *text)
{
    uint8_t temp[16];
    for (int i = 0; i < 16; i++)
        temp[i] = text[SHIFTROWS_TABLE[i]];
    memcpy(text, temp, 16 * sizeof(uint8_t));
}

void mixColumns(uint8_t *text)
{
    uint8_t result[16];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            uint8_t mixed_num = 0;
            for (int k = 0; k < 4; k++) {
                if (MIXCOLUMNS_MATRIX[i][k] == 1)
                    mixed_num ^= text[k * 4 + j];
                else if (MIXCOLUMNS_MATRIX[i][k] == 2)
                    mixed_num ^= ((text[k * 4 + j] << 1) ^ ((text[k * 4 + j] >> 7) * 0x1B)) ^ 0xff;
                else if (MIXCOLUMNS_MATRIX[i][k] == 3)
                    mixed_num ^= ((text[k * 4 + j] << 1) ^ ((text[k * 4 + j] >> 7) * 0x1B) ^ text[k * 4 + j]) ^ 0xff; 
            }
            result[i*4 +j] = mixed_num;
        }
    }
    memcpy(text, result, 16 * sizeof(uint8_t));
}
