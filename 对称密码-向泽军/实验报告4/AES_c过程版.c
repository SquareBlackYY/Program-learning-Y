#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// 字节代换S盒
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

// 行移位映射表
const int SHIFTROWS_TABLE[16] = {
    0, 5, 10, 15,
    4, 9, 14, 3,
    8, 13, 2, 7,
    12, 1, 6, 11};

// 列混淆矩阵
const uint8_t MIXCOLUMNS_MATRIX[4][4] = {
    {0x02, 0x03, 0x01, 0x01},
    {0x01, 0x02, 0x03, 0x01},
    {0x01, 0x01, 0x02, 0x03},
    {0x03, 0x01, 0x01, 0x02}};

// 轮常量
const uint32_t R_CON[10] = {
    0x00000001, 0x00000002, 0x00000004, 0x00000008, 0x00000010,
    0x00000020, 0x00000040, 0x00000080, 0x0000001B, 0x00000036};

void read_input(uint8_t ***, int *); // 读入
void write_output(uint8_t **, int);  // 输出

void KeySchedule(char *, uint32_t ***); // 扩展密钥
void RotWord(uint8_t *);
void SubWord(uint8_t *);

void AES_encrypt(int, uint8_t **, uint32_t **); // 加密函数
void SubBytes(uint8_t *);                       // 字节代换
void ShiftRows(uint8_t *);                      // 行移位
void MixColumns(uint8_t *);                     // 列混淆
void AddRoundKey(uint32_t *, uint32_t *);       // 轮密钥加

void ta(uint8_t *array)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
            printf(" %02x", array[j * 4 + i]);
        printf("\n");
    }
    printf("\n");
}

int main()
{
    uint32_t **key_schedule;
    char *seed_key = "01010101010101010101010101010101";
    KeySchedule(seed_key, &key_schedule);

    uint8_t **text;
    int num_groups;
    read_input(&text, &num_groups);

    clock_t start_time, end_time;
    double execution_time;

    start_time = clock();

    AES_encrypt(num_groups, text, key_schedule);

    end_time = clock();
    execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("程序用时: %.2f s\n", execution_time);
    printf("平均速度: %.2f Mbps\n", 128 * num_groups / execution_time / 1024 / 1024);

    write_output(text, num_groups);

    for (int i = 0; i < 11; i++)
        free(key_schedule[i]);
    free(key_schedule);
    for (int i = 0; i < num_groups; i++)
        free(text[i]);
    free(text);

    return 0;
}

void KeySchedule(char *seed_key, uint32_t ***key_schedule)
{
    *key_schedule = (uint32_t **)malloc(11 * sizeof(uint32_t *));
    for (int i = 0; i < 11; i++)
        (*key_schedule)[i] = (uint32_t *)malloc(4 * sizeof(uint32_t));

    char hex_text[10];
    for (int i = 0; i < 16; i++)
    {
        sscanf(seed_key + (i * 2), "%2s", hex_text);
        ((uint8_t **)(*key_schedule))[0][i] = (uint8_t)strtol(hex_text, NULL, 16);
    }

    for (int round_num = 1; round_num < 11; round_num++)
    {
        memcpy((*key_schedule)[round_num], (*key_schedule)[round_num - 1] + 3, sizeof(uint32_t));
        RotWord((uint8_t *)(*key_schedule)[round_num]);
        SubWord((uint8_t *)(*key_schedule)[round_num]);
        (*key_schedule)[round_num][0] ^= (*key_schedule)[round_num - 1][0] ^ R_CON[round_num - 1];
        for (int i = 1; i < 4; i++)
            (*key_schedule)[round_num][i] = (*key_schedule)[round_num][i - 1] ^ (*key_schedule)[round_num - 1][i];
    }

    printf("密钥扩展完成。\n");
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

void AES_encrypt(int num_groups, uint8_t **input_text, uint32_t **key_schedule)
{
    uint8_t *text;
    for (int i = 0; i < num_groups; i++)
    {
        text = input_text[i];

        printf("Input:\n");
        ta(text);
        printf("SeedKey:\n");
        ta((uint8_t *)key_schedule[0]);

        AddRoundKey((uint32_t *)text, key_schedule[0]);
        for (int round_num = 1; round_num < 10; round_num++)
        {
            printf("第%d轮 Start:\n", round_num);
            ta(text);

            SubBytes(text);

            printf("第%d轮 After SB:\n", round_num);
            ta(text);

            ShiftRows(text);

            printf("第%d轮 After SR:\n", round_num);
            ta(text);

            MixColumns(text);

            printf("第%d轮 After MC:\n", round_num);
            ta(text);

            printf("第%d轮 Key:\n", round_num);
            ta((uint8_t *)key_schedule[round_num]);

            AddRoundKey((uint32_t *)text, key_schedule[round_num]);
        }

        printf("第10轮 Start:\n");
        ta(text);

        SubBytes(text);

        printf("第10轮 After SB:\n");
        ta(text);

        ShiftRows(text);

        printf("第10轮 After SR:\n");
        ta(text);
        printf("第10轮 Key:\n");
        ta((uint8_t *)key_schedule[10]);

        AddRoundKey((uint32_t *)text, key_schedule[10]);

        printf("Output:\n");
        ta(text);
    }
}

void SubBytes(uint8_t *text)
{
    for (int i = 0; i < 16; i++)
        text[i] = S_BOX[text[i]];
}

void ShiftRows(uint8_t *text)
{
    uint8_t result[16];
    for (int i = 0; i < 16; i++)
        result[i] = text[SHIFTROWS_TABLE[i]];
    memcpy(text, result, 16 * sizeof(uint8_t));
}

void MixColumns(uint8_t *text)
{
    uint8_t result[16];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            uint8_t mixed_num = 0;
            for (int k = 0; k < 4; k++)
            {
                if (MIXCOLUMNS_MATRIX[j][k] == 1)
                    mixed_num ^= text[i * 4 + k];
                else if (MIXCOLUMNS_MATRIX[j][k] == 2)
                    mixed_num ^= ((text[i * 4 + k] << 1) ^ ((text[i * 4 + k] >> 7) * 0x1B)) & 0xFF;
                else if (MIXCOLUMNS_MATRIX[j][k] == 3)
                    mixed_num ^= ((text[i * 4 + k] << 1) ^ ((text[i * 4 + k] >> 7) * 0x1B) ^ text[i * 4 + k]) & 0xFF;
            }
            result[i * 4 + j] = mixed_num;
        }
    }
    memcpy(text, result, 16 * sizeof(uint8_t));
}

void AddRoundKey(uint32_t *text, uint32_t *key)
{
    for (int i = 0; i < 4; i++)
        text[i] ^= key[i];
}

void read_input(uint8_t ***text, int *num_groups)
{
    FILE *input_file = fopen("input.txt", "r");
    fseek(input_file, 0, SEEK_END);
    *num_groups = ftell(input_file) / 32;
    rewind(input_file);
    printf("文件大小: %.2f MB\n", 16 * (*num_groups) / 1024.0 / 1024);

    *text = (uint8_t **)malloc(*num_groups * sizeof(uint8_t *));
    for (int i = 0; i < *num_groups; i++)
        (*text)[i] = (uint8_t *)malloc(16 * sizeof(uint8_t));

    char hex_text[3];
    for (int i = 0; i < *num_groups; i++)
        for (int j = 0; j < 16; j++)
        {
            fread(hex_text, 2, 1, input_file);
            hex_text[2] = '\0';
            (*text)[i][j] = (uint8_t)strtol(hex_text, NULL, 16);
        }

    fclose(input_file);
    printf("文件读取完成。\n");
}

void write_output(uint8_t **text, int num_groups)
{
    FILE *output_file = fopen("output.txt", "w");
    for (int i = 0; i < num_groups; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            uint8_t value = text[i][j];
            uint8_t high_nibble = (value >> 4) & 0xF;
            uint8_t low_nibble = value & 0xF;
            char high_hex = (high_nibble < 10) ? ('0' + high_nibble) : ('A' + high_nibble - 10);
            char low_hex = (low_nibble < 10) ? ('0' + low_nibble) : ('A' + low_nibble - 10);
            fprintf(output_file, "%c%c", high_hex, low_hex);
        }
    }
    fclose(output_file);
    printf("文件写入完成。\n");
}