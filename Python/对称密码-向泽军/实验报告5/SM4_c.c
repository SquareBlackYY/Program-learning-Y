#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

const uint32_t FK[4] = {0xA3B1BAC6, 0x56AA3350, 0x677D9197, 0xB27022DC};

const uint32_t CK[32] = {
    0x00070e15, 0x1c232a31, 0x383f464d, 0x545b6269,
    0x70777e85, 0x8c939aa1, 0xa8afb6bd, 0xc4cbd2d9,
    0xe0e7eef5, 0xfc030a11, 0x181f262d, 0x343b4249,
    0x50575e65, 0x6c737a81, 0x888f969d, 0xa4abb2b9,
    0xc0c7ced5, 0xdce3eaf1, 0xf8ff060d, 0x141b2229,
    0x30373e45, 0x4c535a61, 0x686f767d, 0x848b9299,
    0xa0a7aeb5, 0xbcc3cad1, 0xd8dfe6ed, 0xf4fb0209,
    0x10171e25, 0x2c333a41, 0x484f565d, 0x646b7279};

const uint8_t S_BOX[256] = {
    0xd6, 0x90, 0xe9, 0xfe, 0xcc, 0xe1, 0x3d, 0xb7, 0x16, 0xb6, 0x14, 0xc2, 0x28, 0xfb, 0x2c, 0x05,
    0x2b, 0x67, 0x9a, 0x76, 0x2a, 0xbe, 0x04, 0xc3, 0xaa, 0x44, 0x13, 0x26, 0x49, 0x86, 0x06, 0x99,
    0x9c, 0x42, 0x50, 0xf4, 0x91, 0xef, 0x98, 0x7a, 0x33, 0x54, 0x0b, 0x43, 0xed, 0xcf, 0xac, 0x62,
    0xe4, 0xb3, 0x1c, 0xa9, 0xc9, 0x08, 0xe8, 0x95, 0x80, 0xdf, 0x94, 0xfa, 0x75, 0x8f, 0x3f, 0xa6,
    0x47, 0x07, 0xa7, 0xfc, 0xf3, 0x73, 0x17, 0xba, 0x83, 0x59, 0x3c, 0x19, 0xe6, 0x85, 0x4f, 0xa8,
    0x68, 0x6b, 0x81, 0xb2, 0x71, 0x64, 0xda, 0x8b, 0xf8, 0xeb, 0x0f, 0x4b, 0x70, 0x56, 0x9d, 0x35,
    0x1e, 0x24, 0x0e, 0x5e, 0x63, 0x58, 0xd1, 0xa2, 0x25, 0x22, 0x7c, 0x3b, 0x01, 0x21, 0x78, 0x87,
    0xd4, 0x00, 0x46, 0x57, 0x9f, 0xd3, 0x27, 0x52, 0x4c, 0x36, 0x02, 0xe7, 0xa0, 0xc4, 0xc8, 0x9e,
    0xea, 0xbf, 0x8a, 0xd2, 0x40, 0xc7, 0x38, 0xb5, 0xa3, 0xf7, 0xf2, 0xce, 0xf9, 0x61, 0x15, 0xa1,
    0xe0, 0xae, 0x5d, 0xa4, 0x9b, 0x34, 0x1a, 0x55, 0xad, 0x93, 0x32, 0x30, 0xf5, 0x8c, 0xb1, 0xe3,
    0x1d, 0xf6, 0xe2, 0x2e, 0x82, 0x66, 0xca, 0x60, 0xc0, 0x29, 0x23, 0xab, 0x0d, 0x53, 0x4e, 0x6f,
    0xd5, 0xdb, 0x37, 0x45, 0xde, 0xfd, 0x8e, 0x2f, 0x03, 0xff, 0x6a, 0x72, 0x6d, 0x6c, 0x5b, 0x51,
    0x8d, 0x1b, 0xaf, 0x92, 0xbb, 0xdd, 0xbc, 0x7f, 0x11, 0xd9, 0x5c, 0x41, 0x1f, 0x10, 0x5a, 0xd8,
    0x0a, 0xc1, 0x31, 0x88, 0xa5, 0xcd, 0x7b, 0xbd, 0x2d, 0x74, 0xd0, 0x12, 0xb8, 0xe5, 0xb4, 0xb0,
    0x89, 0x69, 0x97, 0x4a, 0x0c, 0x96, 0x77, 0x7e, 0x65, 0xb9, 0xf1, 0x09, 0xc5, 0x6e, 0xc6, 0x84,
    0x18, 0xf0, 0x7d, 0xec, 0x3a, 0xdc, 0x4d, 0x20, 0x79, 0xee, 0x5f, 0x3e, 0xd7, 0xcb, 0x39, 0x48};

void read_input(uint32_t ***, int *);
//void write_output(uint32_t **, int);
void generate_key_schedule(char *, uint32_t **);
void key_extend(uint32_t *);
uint32_t T_function_for_key(uint32_t);
void AES_encrypt(int, uint32_t **, uint32_t *);
uint32_t T_function_for_word(uint32_t);

int main()
{
    uint32_t *key_schedule;
    char *MK = "01010101010101010101010101010101";
    generate_key_schedule(MK, &key_schedule);
    
    uint32_t **text;
    int num_groups;
    read_input(&text, &num_groups);

    AES_encrypt(num_groups, text, key_schedule);

    free(key_schedule);
    for (int i = 0; i < num_groups; i++)
        free(text[i]);
    free(text);

    return 0;
}

void AES_encrypt(int num_groups, uint32_t **input_text, uint32_t *key_schedule)
{
    uint32_t *text;
    uint32_t temp;
    uint32_t *rK = &key_schedule[4];
    for (int i = 0; i < num_groups; i++)
    {
        text = input_text[i];
        for (int round_num = 0; round_num < 32; round_num++)
            temp = text[i] ^ T_function_for_word(text[i + 1] ^ text[i + 2] ^ text[i + 3] ^ rK[i] & 0xFFFFFFFF) & 0xFFFFFFFF;
        memcpy(text, text + 1, 3 * sizeof(uint32_t));
        text[3] = temp;
    }
}

uint32_t T_function_for_word(uint32_t word)
{
    uint8_t *p = (uint8_t *)&word;
    for (int i = 0; i < 4; i++)
        p[i] = S_BOX[p[i]];
    word = word ^ ((word << 2) | (word >> 30)) ^ ((word << 10) | (word >> 22)) ^ ((word << 18) | (word >> 14)) ^ ((word << 24) | (word >> 8)) & 0xFFFFFFFF;
    return word;
}

void generate_key_schedule(char *MK, uint32_t **key_schedule)
{
    char hex_text[9];
    *key_schedule = (uint32_t *)malloc(36 * sizeof(uint32_t));

    for (int i = 0; i < 4; i++)
    {
        sscanf(MK + (i * 8), "%8s", hex_text);
        (*key_schedule)[i] = (uint32_t)strtol(hex_text, NULL, 16) ^ FK[i] & 0xFFFFFFFF;
    }
    key_extend(*key_schedule);
}

void key_extend(uint32_t *key_schedule)
{
    for (int i = 0; i < 32; i++)
        key_schedule[i + 4] = key_schedule[i] ^ T_function_for_key(key_schedule[i + 1] ^ key_schedule[i + 2] ^ key_schedule[i + 3] ^ CK[i] & 0xFFFFFFFF) & 0xFFFFFFFF;
}

uint32_t T_function_for_key(uint32_t word)
{
    uint8_t *p = (uint8_t *)&word;
    for (int i = 0; i < 4; i++)
        p[i] = S_BOX[p[i]];
    word = word ^ ((word << 13) | (word >> 19)) ^ ((word << 23) | (word >> 9)) & 0xFFFFFFFF;
    return word;
}

void read_input(uint32_t ***text, int *num_groups)
{
    FILE *input_file = fopen("input.txt", "r");
    fseek(input_file, 0, SEEK_END);
    *num_groups = ftell(input_file) / 32;
    rewind(input_file);
    printf("文件大小: %.2f MB\n", 16 * (*num_groups) / 1024.0 / 1024);

    *text = (uint32_t **)malloc(*num_groups * sizeof(uint32_t *));
    for (int i = 0; i < *num_groups; i++)
        (*text)[i] = (uint32_t *)malloc(4 * sizeof(uint32_t));

    char hex_text[9];
    for (int i = 0; i < *num_groups; i++)
        for (int j = 0; j < 4; j++)
        {
            fread(hex_text, 8, 1, input_file);
            hex_text[8] = '\0';
            (*text)[i][j] = (uint8_t)strtol(hex_text, NULL, 16);
        }

    fclose(input_file);
    printf("文件读取完成。\n");
}

// 没改好
/*
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
*/