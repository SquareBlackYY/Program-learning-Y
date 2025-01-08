#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define AES_BLOCK_SIZE 16            // AES分组长度（字节）
#define AES_KEY_SIZE 16              // AES种子密钥长度（字节）
#define AES_ROUND_KEY_SIZE 11        // AES扩展密钥轮数
#define TEST_BLOCK_SIZE (1ULL << 20) // 测试分组（16字节）

// S盒 用于加密时字节代换
static const uint8_t S_BOX[256] = {
    // 0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, // 0
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, // 1
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, // 2
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, // 3
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, // 4
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, // 5
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, // 6
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, // 7
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, // 8
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, // 9
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, // A
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, // B
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, // C
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, // D
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, // E
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16  // F
};

// 逆S盒 用于解密时字节代换
static const uint8_t INV_S_BOX[256] = {
    // 0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb, // 0
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb, // 1
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e, // 2
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25, // 3
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92, // 4
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84, // 5
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06, // 6
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b, // 7
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73, // 8
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e, // 9
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b, // A
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4, // B
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f, // C
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef, // D
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61, // E
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d  // F
};

// 轮常量 用于密钥扩展
static const uint8_t R_CON[11] = {
    0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};

// xtime表 用于有限域 GF(2^8) 上的乘法运算
static const uint8_t XTIME_TABLE[256] = {
    0x00, 0x02, 0x04, 0x06, 0x08, 0x0a, 0x0c, 0x0e, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1a, 0x1c, 0x1e,
    0x20, 0x22, 0x24, 0x26, 0x28, 0x2a, 0x2c, 0x2e, 0x30, 0x32, 0x34, 0x36, 0x38, 0x3a, 0x3c, 0x3e,
    0x40, 0x42, 0x44, 0x46, 0x48, 0x4a, 0x4c, 0x4e, 0x50, 0x52, 0x54, 0x56, 0x58, 0x5a, 0x5c, 0x5e,
    0x60, 0x62, 0x64, 0x66, 0x68, 0x6a, 0x6c, 0x6e, 0x70, 0x72, 0x74, 0x76, 0x78, 0x7a, 0x7c, 0x7e,
    0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c, 0x8e, 0x90, 0x92, 0x94, 0x96, 0x98, 0x9a, 0x9c, 0x9e,
    0xa0, 0xa2, 0xa4, 0xa6, 0xa8, 0xaa, 0xac, 0xae, 0xb0, 0xb2, 0xb4, 0xb6, 0xb8, 0xba, 0xbc, 0xbe,
    0xc0, 0xc2, 0xc4, 0xc6, 0xc8, 0xca, 0xcc, 0xce, 0xd0, 0xd2, 0xd4, 0xd6, 0xd8, 0xda, 0xdc, 0xde,
    0xe0, 0xe2, 0xe4, 0xe6, 0xe8, 0xea, 0xec, 0xee, 0xf0, 0xf2, 0xf4, 0xf6, 0xf8, 0xfa, 0xfc, 0xfe,
    0x1b, 0x19, 0x1f, 0x1d, 0x13, 0x11, 0x17, 0x15, 0x0b, 0x09, 0x0f, 0x0d, 0x03, 0x01, 0x07, 0x05,
    0x3b, 0x39, 0x3f, 0x3d, 0x33, 0x31, 0x37, 0x35, 0x2b, 0x29, 0x2f, 0x2d, 0x23, 0x21, 0x27, 0x25,
    0x5b, 0x59, 0x5f, 0x5d, 0x53, 0x51, 0x57, 0x55, 0x4b, 0x49, 0x4f, 0x4d, 0x43, 0x41, 0x47, 0x45,
    0x7b, 0x79, 0x7f, 0x7d, 0x73, 0x71, 0x77, 0x75, 0x6b, 0x69, 0x6f, 0x6d, 0x63, 0x61, 0x67, 0x65,
    0x9b, 0x99, 0x9f, 0x9d, 0x93, 0x91, 0x97, 0x95, 0x8b, 0x89, 0x8f, 0x8d, 0x83, 0x81, 0x87, 0x85,
    0xbb, 0xb9, 0xbf, 0xbd, 0xb3, 0xb1, 0xb7, 0xb5, 0xab, 0xa9, 0xaf, 0xad, 0xa3, 0xa1, 0xa7, 0xa5,
    0xdb, 0xd9, 0xdf, 0xdd, 0xd3, 0xd1, 0xd7, 0xd5, 0xcb, 0xc9, 0xcf, 0xcd, 0xc3, 0xc1, 0xc7, 0xc5,
    0xfb, 0xf9, 0xff, 0xfd, 0xf3, 0xf1, 0xf7, 0xf5, 0xeb, 0xe9, 0xef, 0xed, 0xe3, 0xe1, 0xe7, 0xe5
};

// 加密查表
static uint32_t ET[4][256];

// 解密查表
static uint32_t DT[4][256];

void aes_key_schedule(const uint8_t *seedKey, uint8_t roundKeys[][16]);
void aes_encrypt(uint8_t *plainText, const uint8_t roundKeys[][16], uint8_t *cipherText);
void aes_decrypt(uint8_t *cipherText, const uint8_t mixRoundKeys[][16], uint8_t *decryptedText);
void add_round_key(uint8_t *state, const uint8_t *roundKey);
void sub_bytes(uint8_t *state);
void inv_sub_bytes(uint8_t *state);
void shift_rows(uint8_t *state);
void inv_shift_rows(uint8_t *state);
uint8_t multiply(uint8_t x, uint8_t y);
void init_encrypt_table();
void init_decrypt_table();
void gen_decrypt_key(uint8_t roundKeys[][16], uint8_t mixRoundKeys[][16]);

int main()
{
    // 种子密钥
    const uint8_t seed_key[AES_KEY_SIZE] = {
        0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff};

    // 明文分组
    uint8_t plain_text[AES_BLOCK_SIZE] = {
        0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff};

    // 密文分组
    uint8_t cipher_text[AES_BLOCK_SIZE];

    // 轮密钥 11轮，每轮16字节
    uint8_t round_keys[AES_ROUND_KEY_SIZE][AES_BLOCK_SIZE];

    // 列混淆处理后的轮密钥 11轮，每轮16字节
    uint8_t mix_round_keys[AES_ROUND_KEY_SIZE][AES_BLOCK_SIZE];

    // 密钥扩展
    aes_key_schedule(seed_key, round_keys);

    // 初始化查表
    init_encrypt_table();
    init_decrypt_table();
    gen_decrypt_key(round_keys, mix_round_keys);

    printf("输入明文:\t");
    for (int i = 0; i < AES_BLOCK_SIZE; i++)
        printf("%02x ", plain_text[i]);
    printf("\n");

    const clock_t start = clock();
    // 加密
    for (int i = 0; i < TEST_BLOCK_SIZE; ++i) {
    aes_encrypt(plain_text, round_keys, cipher_text);
    }
    const clock_t end = clock();

    printf("加密结果:\t");
    for (int i = 0; i < AES_BLOCK_SIZE; i++)
        printf("%02x ", plain_text[i]);
    printf("\n");

    // 测试数据大小（MB）
    const int data = 16 * TEST_BLOCK_SIZE / 1024 / 1024;
    printf("加密了 %d MB 数据, 速度: %.2f Mbps\n", data, 8 * data / ((double)(end - start) / CLOCKS_PER_SEC));

    // 解密
    // for (int i = 0; i < BLOCK_SIZE; ++i) {
    aes_decrypt(plain_text, mix_round_keys, cipher_text);
    // }

    printf("解密结果:\t");
    for (int i = 0; i < AES_BLOCK_SIZE; i++)
        printf("%02x ", plain_text[i]);
    printf("\n");

    return 0;
}

// 密钥扩展
void aes_key_schedule(const uint8_t *seedKey, uint8_t roundKeys[][16])
{
    // Copy initial key
    for (int i = 0; i < AES_KEY_SIZE; ++i)
        roundKeys[0][i] = seedKey[i];

    int rcon_index = 1;

    for (int i = 1; i < 11; ++i)
    {
        roundKeys[i][0] = S_BOX[roundKeys[i - 1][13]] ^ R_CON[rcon_index++] ^ roundKeys[i - 1][0];
        roundKeys[i][1] = S_BOX[roundKeys[i - 1][14]] ^ roundKeys[i - 1][1];
        roundKeys[i][2] = S_BOX[roundKeys[i - 1][15]] ^ roundKeys[i - 1][2];
        roundKeys[i][3] = S_BOX[roundKeys[i - 1][12]] ^ roundKeys[i - 1][3];

        for (int j = 4; j < 16; ++j)
        {
            roundKeys[i][j] = roundKeys[i][j - 4] ^ roundKeys[i - 1][j];
        }
    }
}

// AES 加密
void aes_encrypt(uint8_t *plainText, const uint8_t roundKeys[][16], uint8_t *cipherText)
{
    uint32_t *u32pt = (uint32_t *)plainText;
    uint32_t *u32ct = (uint32_t *)cipherText;
    uint32_t *key = (uint32_t *)roundKeys[0];
    uint32_t *t;

    u32pt[0] ^= key[0];
    u32pt[1] ^= key[1];
    u32pt[2] ^= key[2];
    u32pt[3] ^= key[3];

    for (int r = 1; r <= 9; ++r)
    {
        key = (uint32_t *)roundKeys[r];
        u32ct[0] = ET[0][plainText[0]] ^ ET[1][plainText[5]] ^ ET[2][plainText[10]] ^ ET[3][plainText[15]] ^ key[0];
        u32ct[1] = ET[0][plainText[4]] ^ ET[1][plainText[9]] ^ ET[2][plainText[14]] ^ ET[3][plainText[3]] ^ key[1];
        u32ct[2] = ET[0][plainText[8]] ^ ET[1][plainText[13]] ^ ET[2][plainText[2]] ^ ET[3][plainText[7]] ^ key[2];
        u32ct[3] = ET[0][plainText[12]] ^ ET[1][plainText[1]] ^ ET[2][plainText[6]] ^ ET[3][plainText[11]] ^ key[3];

        t = u32ct;
        u32ct = u32pt;
        u32pt = t;
        plainText = (uint8_t *)u32pt;
    }
    cipherText = (uint8_t *)u32ct;

    cipherText[0] = S_BOX[plainText[0]] ^ roundKeys[10][0];
    cipherText[1] = S_BOX[plainText[5]] ^ roundKeys[10][1];
    cipherText[2] = S_BOX[plainText[10]] ^ roundKeys[10][2];
    cipherText[3] = S_BOX[plainText[15]] ^ roundKeys[10][3];

    cipherText[4] = S_BOX[plainText[4]] ^ roundKeys[10][4];
    cipherText[5] = S_BOX[plainText[9]] ^ roundKeys[10][5];
    cipherText[6] = S_BOX[plainText[14]] ^ roundKeys[10][6];
    cipherText[7] = S_BOX[plainText[3]] ^ roundKeys[10][7];

    cipherText[8] = S_BOX[plainText[8]] ^ roundKeys[10][8];
    cipherText[9] = S_BOX[plainText[13]] ^ roundKeys[10][9];
    cipherText[10] = S_BOX[plainText[2]] ^ roundKeys[10][10];
    cipherText[11] = S_BOX[plainText[7]] ^ roundKeys[10][11];

    cipherText[12] = S_BOX[plainText[12]] ^ roundKeys[10][12];
    cipherText[13] = S_BOX[plainText[1]] ^ roundKeys[10][13];
    cipherText[14] = S_BOX[plainText[6]] ^ roundKeys[10][14];
    cipherText[15] = S_BOX[plainText[11]] ^ roundKeys[10][15];
}

// AES 解密
void aes_decrypt(uint8_t *cipherText, const uint8_t mixRoundKeys[][16], uint8_t *decryptedText)
{
    uint32_t *u32ct = (uint32_t *)cipherText;
    uint32_t *u32dt = (uint32_t *)decryptedText;
    uint32_t *key = (uint32_t *)mixRoundKeys[10];
    uint32_t *t;

    u32ct[0] ^= key[0];
    u32ct[1] ^= key[1];
    u32ct[2] ^= key[2];
    u32ct[3] ^= key[3];

    for (int r = 9; r >= 1; --r)
    {
        key = (uint32_t *)mixRoundKeys[r];
        u32dt[0] = DT[0][cipherText[0]] ^ DT[1][cipherText[13]] ^ DT[2][cipherText[10]] ^ DT[3][cipherText[7]] ^ key[0];
        u32dt[1] = DT[0][cipherText[4]] ^ DT[1][cipherText[1]] ^ DT[2][cipherText[14]] ^ DT[3][cipherText[11]] ^ key[1];
        u32dt[2] = DT[0][cipherText[8]] ^ DT[1][cipherText[5]] ^ DT[2][cipherText[2]] ^ DT[3][cipherText[15]] ^ key[2];
        u32dt[3] = DT[0][cipherText[12]] ^ DT[1][cipherText[9]] ^ DT[2][cipherText[6]] ^ DT[3][cipherText[3]] ^ key[3];

        t = u32dt;
        u32dt = u32ct;
        u32ct = t;
        cipherText = (uint8_t *)u32ct;
    }
    decryptedText = (uint8_t *)u32dt;

    decryptedText[0] = INV_S_BOX[cipherText[0]] ^ mixRoundKeys[0][0];
    decryptedText[1] = INV_S_BOX[cipherText[13]] ^ mixRoundKeys[0][1];
    decryptedText[2] = INV_S_BOX[cipherText[10]] ^ mixRoundKeys[0][2];
    decryptedText[3] = INV_S_BOX[cipherText[7]] ^ mixRoundKeys[0][3];

    decryptedText[4] = INV_S_BOX[cipherText[4]] ^ mixRoundKeys[0][4];
    decryptedText[5] = INV_S_BOX[cipherText[1]] ^ mixRoundKeys[0][5];
    decryptedText[6] = INV_S_BOX[cipherText[14]] ^ mixRoundKeys[0][6];
    decryptedText[7] = INV_S_BOX[cipherText[11]] ^ mixRoundKeys[0][7];

    decryptedText[8] = INV_S_BOX[cipherText[8]] ^ mixRoundKeys[0][8];
    decryptedText[9] = INV_S_BOX[cipherText[5]] ^ mixRoundKeys[0][9];
    decryptedText[10] = INV_S_BOX[cipherText[2]] ^ mixRoundKeys[0][10];
    decryptedText[11] = INV_S_BOX[cipherText[15]] ^ mixRoundKeys[0][11];

    decryptedText[12] = INV_S_BOX[cipherText[12]] ^ mixRoundKeys[0][12];
    decryptedText[13] = INV_S_BOX[cipherText[9]] ^ mixRoundKeys[0][13];
    decryptedText[14] = INV_S_BOX[cipherText[6]] ^ mixRoundKeys[0][14];
    decryptedText[15] = INV_S_BOX[cipherText[3]] ^ mixRoundKeys[0][15];
}

// 轮密钥加
void add_round_key(uint8_t *state, const uint8_t *roundKey)
{
    for (int i = 0; i < AES_BLOCK_SIZE; i++)
        state[i] ^= roundKey[i];
}

// 字节代换
void sub_bytes(uint8_t *state)
{
    for (int i = 0; i < AES_BLOCK_SIZE; i++)
        state[i] = S_BOX[state[i]];
}

// 逆字节代换
void inv_sub_bytes(uint8_t *state)
{
    for (int i = 0; i < AES_BLOCK_SIZE; i++)
        state[i] = INV_S_BOX[state[i]];
}

// 行移位
void shift_rows(uint8_t *state)
{
    uint8_t temp;

    // Shift row 1 left by 1
    temp = state[1];
    state[1] = state[5];
    state[5] = state[9];
    state[9] = state[13];
    state[13] = temp;

    // Shift row 2 left by 2
    temp = state[2];
    state[2] = state[10];
    state[10] = temp;
    temp = state[6];
    state[6] = state[14];
    state[14] = temp;

    // Shift row 3 left by 3
    temp = state[3];
    state[3] = state[15];
    state[15] = state[11];
    state[11] = state[7];
    state[7] = temp;
}

// 逆行移位
void inv_shift_rows(uint8_t *state)
{
    uint8_t temp;

    // Inverse shift row 1 right by 1
    temp = state[13];
    state[13] = state[9];
    state[9] = state[5];
    state[5] = state[1];
    state[1] = temp;

    // Inverse shift row 2 right by 2
    temp = state[10];
    state[10] = state[2];
    state[2] = temp;
    temp = state[14];
    state[14] = state[6];
    state[6] = temp;

    // Inverse shift row 3 right by 3
    temp = state[7];
    state[7] = state[3];
    state[3] = state[15];
    state[15] = state[11];
    state[11] = temp;
}

// 两个字节在有限域 GF(2^8) 上的乘法运算
uint8_t multiply(uint8_t x, uint8_t y)
{
    uint8_t result = 0;
    uint8_t temp = y;

    while (x)
    {
        if (x & 0x01)
            result ^= temp;
        temp = XTIME_TABLE[temp];
        x >>= 1;
    }

    return result;
}

// 初始化加密查表
void init_encrypt_table()
{
    for (int j = 0; j < 256; ++j)
    {
        ET[0][j] = (uint32_t)multiply(0x03, S_BOX[j]) << 24 | (uint32_t)S_BOX[j] << 16 | (uint32_t)S_BOX[j] << 8 | (uint32_t)multiply(0x02, S_BOX[j]);
        ET[1][j] = ET[0][j] << 8 | ET[0][j] >> 24;
        ET[2][j] = ET[1][j] << 8 | ET[1][j] >> 24;
        ET[3][j] = ET[2][j] << 8 | ET[2][j] >> 24;
    }
}

// 初始化解密查表
void init_decrypt_table()
{
    for (int j = 0; j < 256; ++j)
    {
        DT[0][j] = (uint32_t)multiply(0x0b, INV_S_BOX[j]) << 24 | (uint32_t)multiply(0x0d, INV_S_BOX[j]) << 16 | (uint32_t)multiply(0x09, INV_S_BOX[j]) << 8 | (uint32_t)multiply(0x0e, INV_S_BOX[j]);
        DT[1][j] = DT[0][j] << 8 | DT[0][j] >> 24;
        DT[2][j] = DT[1][j] << 8 | DT[1][j] >> 24;
        DT[3][j] = DT[2][j] << 8 | DT[2][j] >> 24;
    }
}

// 生成解密轮密钥
void gen_decrypt_key(uint8_t roundKeys[11][16], uint8_t mixRoundKeys[11][16])
{
    // First round
    for (int i = 0; i < 16; ++i)
        mixRoundKeys[0][i] = roundKeys[0][i];

    for (int i = 1; i <= 9; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            int idx = 4 * j;
            mixRoundKeys[i][idx] = multiply(0x0e, roundKeys[i][idx]) ^ multiply(0x0b, roundKeys[i][idx + 1]) ^ multiply(0x0d, roundKeys[i][idx + 2]) ^ multiply(0x09, roundKeys[i][idx + 3]);
            mixRoundKeys[i][idx + 1] = multiply(0x09, roundKeys[i][idx]) ^ multiply(0x0e, roundKeys[i][idx + 1]) ^ multiply(0x0b, roundKeys[i][idx + 2]) ^ multiply(0x0d, roundKeys[i][idx + 3]);
            mixRoundKeys[i][idx + 2] = multiply(0x0d, roundKeys[i][idx]) ^ multiply(0x09, roundKeys[i][idx + 1]) ^ multiply(0x0e, roundKeys[i][idx + 2]) ^ multiply(0x0b, roundKeys[i][idx + 3]);
            mixRoundKeys[i][idx + 3] = multiply(0x0b, roundKeys[i][idx]) ^ multiply(0x0d, roundKeys[i][idx + 1]) ^ multiply(0x09, roundKeys[i][idx + 2]) ^ multiply(0x0e, roundKeys[i][idx + 3]);
        }
    }

    // Final round
    for (int i = 0; i < 16; ++i)
        mixRoundKeys[10][i] = roundKeys[10][i];
}

