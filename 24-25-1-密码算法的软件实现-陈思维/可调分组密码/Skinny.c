/**
 * Skinny
 * 分组长度-密钥长度
 * 64-64
 * 64-128
 * 64-192
 * 128-128
 * 128-256
 * 128-384
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define BLOCK_SIZE 128                                  // 分组长度
#define TWEAKEY_SIZE 256                                // 密钥长度
#define S (BLOCK_SIZE / 16)                             // 单元大小
#define Z (TWEAKEY_SIZE / BLOCK_SIZE)                   // 密钥长度与分组长度比值
#define ROUNDS (24 + (BLOCK_SIZE + TWEAKEY_SIZE) / 16)  // 轮数

#if BLOCK_SIZE == 64 && TWEAKEY_SIZE == 64
#define SKINNY_TEST_KEY { \
0xf5269826fc681238ULL}
#define SKINNY_TEST_PLAINTEXT { \
0x06034f957724d19dULL}
#define SKINNY_TEST_CIPHERTEXT { \
0xbb39dfb2429b8ac7ULL}
#elif BLOCK_SIZE == 64 && TWEAKEY_SIZE == 128
#define SKINNY_TEST_KEY { \
0x9eb93640d088da63ULL, 0x76a39d1c8bea71e1ULL}
#define SKINNY_TEST_PLAINTEXT { \
0xcf16cfe8fd0f98aaULL}
#define SKINNY_TEST_CIPHERTEXT { \
0x6ceda1f43de92b9eULL}
#elif BLOCK_SIZE == 64 && TWEAKEY_SIZE == 192
#define SKINNY_TEST_KEY { \
0xed00c85b120d6861ULL, 0x8753e24bfd908f60ULL, \
0xb2dbb41b422dfcd0ULL}
#define SKINNY_TEST_PLAINTEXT { \
0x530c61d35e8663c3ULL}
#define SKINNY_TEST_CIPHERTEXT { \
0xdd2cf1a8f330303cULL}
#elif BLOCK_SIZE == 128 && TWEAKEY_SIZE == 128
#define SKINNY_TEST_KEY { \
0x4f55cfb0520cac52ULL, 0xfd92c15f37073e93ULL}
#define SKINNY_TEST_PLAINTEXT { \
0xf20adb0eb08b648aULL, 0x3b2eeed1f0adda14ULL}
#define SKINNY_TEST_CIPHERTEXT { \
0x22ff30d498ea62d7ULL, 0xe45b476e33675b74ULL}
#elif BLOCK_SIZE == 128 && TWEAKEY_SIZE == 256
#define SKINNY_TEST_KEY { \
0x009cec81605d4ac1ULL, 0xd2ae9e3085d7a1f3ULL, \
0x1ac123ebfc00fddcULL, 0xf01046ceeddfcab3ULL}
#define SKINNY_TEST_PLAINTEXT { \
0x3a0c47767a26a68dULL, 0xd382a695e7022e25ULL}
#define SKINNY_TEST_CIPHERTEXT { \
0xb731d98a4bde147aULL, 0x7ed4a6f16b9b587fULL}
#elif BLOCK_SIZE == 128 && TWEAKEY_SIZE == 384
#define SKINNY_TEST_KEY { \
0xdf889548cfc7ea52ULL, 0xd296339301797449ULL, \
0xab588a34a47f1ab2ULL, 0xdfe9c8293fbea9a5ULL, \
0xab1afac2611012cdULL, 0x8cef952618c3ebe8ULL}
#define SKINNY_TEST_PLAINTEXT { \
0xa3994b66ad85a345ULL, 0x9f44e92b08f550cbULL}
#define SKINNY_TEST_CIPHERTEXT { \
0x94ecf589e2017c60ULL, 0x1b38c6346a10dcfaULL}
#endif

typedef uint64_t SkinnyKey[TWEAKEY_SIZE / 64];
typedef uint8_t SkinnyRoundKey[ROUNDS][8];
typedef uint64_t SkinnyBlock[BLOCK_SIZE / 64];

static const int P[16] = {9, 15, 8, 13, 10, 14, 12, 11, 0, 1, 2, 3, 4, 5, 6, 7};

static const uint8_t SBOX_4[16] = {0xc, 0x6, 0x9, 0x0, 0x1, 0xa, 0x2, 0xb, 0x3, 0x8, 0x5, 0xd, 0x4, 0xe, 0x7, 0xf};

static const uint8_t INV_SBOX_4[16] = {0x3, 0x4, 0x6, 0x8, 0xc, 0xa, 0x1, 0xe, 0x9, 0x2, 0x5, 0x7, 0x0, 0xb, 0xd, 0xf};

static const uint8_t SBOX_8[256] = {
    0x65, 0x4c, 0x6a, 0x42, 0x4b, 0x63, 0x43, 0x6b, 0x55, 0x75, 0x5a, 0x7a, 0x53, 0x73, 0x5b, 0x7b,
    0x35, 0x8c, 0x3a, 0x81, 0x89, 0x33, 0x80, 0x3b, 0x95, 0x25, 0x98, 0x2a, 0x90, 0x23, 0x99, 0x2b,
    0xe5, 0xcc, 0xe8, 0xc1, 0xc9, 0xe0, 0xc0, 0xe9, 0xd5, 0xf5, 0xd8, 0xf8, 0xd0, 0xf0, 0xd9, 0xf9,
    0xa5, 0x1c, 0xa8, 0x12, 0x1b, 0xa0, 0x13, 0xa9, 0x05, 0xb5, 0x0a, 0xb8, 0x03, 0xb0, 0x0b, 0xb9,
    0x32, 0x88, 0x3c, 0x85, 0x8d, 0x34, 0x84, 0x3d, 0x91, 0x22, 0x9c, 0x2c, 0x94, 0x24, 0x9d, 0x2d,
    0x62, 0x4a, 0x6c, 0x45, 0x4d, 0x64, 0x44, 0x6d, 0x52, 0x72, 0x5c, 0x7c, 0x54, 0x74, 0x5d, 0x7d,
    0xa1, 0x1a, 0xac, 0x15, 0x1d, 0xa4, 0x14, 0xad, 0x02, 0xb1, 0x0c, 0xbc, 0x04, 0xb4, 0x0d, 0xbd,
    0xe1, 0xc8, 0xec, 0xc5, 0xcd, 0xe4, 0xc4, 0xed, 0xd1, 0xf1, 0xdc, 0xfc, 0xd4, 0xf4, 0xdd, 0xfd,
    0x36, 0x8e, 0x38, 0x82, 0x8b, 0x30, 0x83, 0x39, 0x96, 0x26, 0x9a, 0x28, 0x93, 0x20, 0x9b, 0x29,
    0x66, 0x4e, 0x68, 0x41, 0x49, 0x60, 0x40, 0x69, 0x56, 0x76, 0x58, 0x78, 0x50, 0x70, 0x59, 0x79,
    0xa6, 0x1e, 0xaa, 0x11, 0x19, 0xa3, 0x10, 0xab, 0x06, 0xb6, 0x08, 0xba, 0x00, 0xb3, 0x09, 0xbb,
    0xe6, 0xce, 0xea, 0xc2, 0xcb, 0xe3, 0xc3, 0xeb, 0xd6, 0xf6, 0xda, 0xfa, 0xd3, 0xf3, 0xdb, 0xfb,
    0x31, 0x8a, 0x3e, 0x86, 0x8f, 0x37, 0x87, 0x3f, 0x92, 0x21, 0x9e, 0x2e, 0x97, 0x27, 0x9f, 0x2f,
    0x61, 0x48, 0x6e, 0x46, 0x4f, 0x67, 0x47, 0x6f, 0x51, 0x71, 0x5e, 0x7e, 0x57, 0x77, 0x5f, 0x7f,
    0xa2, 0x18, 0xae, 0x16, 0x1f, 0xa7, 0x17, 0xaf, 0x01, 0xb2, 0x0e, 0xbe, 0x07, 0xb7, 0x0f, 0xbf,
    0xe2, 0xca, 0xee, 0xc6, 0xcf, 0xe7, 0xc7, 0xef, 0xd2, 0xf2, 0xde, 0xfe, 0xd7, 0xf7, 0xdf, 0xff
};

static const uint8_t INV_SBOX_8[256] = {
    0xac, 0xe8, 0x68, 0x3c, 0x6c, 0x38, 0xa8, 0xec, 0xaa, 0xae, 0x3a, 0x3e, 0x6a, 0x6e, 0xea, 0xee,
    0xa6, 0xa3, 0x33, 0x36, 0x66, 0x63, 0xe3, 0xe6, 0xe1, 0xa4, 0x61, 0x34, 0x31, 0x64, 0xa1, 0xe4,
    0x8d, 0xc9, 0x49, 0x1d, 0x4d, 0x19, 0x89, 0xcd, 0x8b, 0x8f, 0x1b, 0x1f, 0x4b, 0x4f, 0xcb, 0xcf,
    0x85, 0xc0, 0x40, 0x15, 0x45, 0x10, 0x80, 0xc5, 0x82, 0x87, 0x12, 0x17, 0x42, 0x47, 0xc2, 0xc7,
    0x96, 0x93, 0x03, 0x06, 0x56, 0x53, 0xd3, 0xd6, 0xd1, 0x94, 0x51, 0x04, 0x01, 0x54, 0x91, 0xd4,
    0x9c, 0xd8, 0x58, 0x0c, 0x5c, 0x08, 0x98, 0xdc, 0x9a, 0x9e, 0x0a, 0x0e, 0x5a, 0x5e, 0xda, 0xde,
    0x95, 0xd0, 0x50, 0x05, 0x55, 0x00, 0x90, 0xd5, 0x92, 0x97, 0x02, 0x07, 0x52, 0x57, 0xd2, 0xd7,
    0x9d, 0xd9, 0x59, 0x0d, 0x5d, 0x09, 0x99, 0xdd, 0x9b, 0x9f, 0x0b, 0x0f, 0x5b, 0x5f, 0xdb, 0xdf,
    0x16, 0x13, 0x83, 0x86, 0x46, 0x43, 0xc3, 0xc6, 0x41, 0x14, 0xc1, 0x84, 0x11, 0x44, 0x81, 0xc4,
    0x1c, 0x48, 0xc8, 0x8c, 0x4c, 0x18, 0x88, 0xcc, 0x1a, 0x1e, 0x8a, 0x8e, 0x4a, 0x4e, 0xca, 0xce,
    0x35, 0x60, 0xe0, 0xa5, 0x65, 0x30, 0xa0, 0xe5, 0x32, 0x37, 0xa2, 0xa7, 0x62, 0x67, 0xe2, 0xe7,
    0x3d, 0x69, 0xe9, 0xad, 0x6d, 0x39, 0xa9, 0xed, 0x3b, 0x3f, 0xab, 0xaf, 0x6b, 0x6f, 0xeb, 0xef,
    0x26, 0x23, 0xb3, 0xb6, 0x76, 0x73, 0xf3, 0xf6, 0x71, 0x24, 0xf1, 0xb4, 0x21, 0x74, 0xb1, 0xf4,
    0x2c, 0x78, 0xf8, 0xbc, 0x7c, 0x28, 0xb8, 0xfc, 0x2a, 0x2e, 0xba, 0xbe, 0x7a, 0x7e, 0xfa, 0xfe,
    0x25, 0x70, 0xf0, 0xb5, 0x75, 0x20, 0xb0, 0xf5, 0x22, 0x27, 0xb2, 0xb7, 0x72, 0x77, 0xf2, 0xf7,
    0x2d, 0x79, 0xf9, 0xbd, 0x7d, 0x29, 0xb9, 0xfd, 0x2b, 0x2f, 0xbb, 0xbf, 0x7b, 0x7f, 0xfb, 0xff
};

static const bool M[16] = {
    1, 0, 1, 1,
    1, 0, 0, 0,
    0, 1, 1, 0,
    1, 0, 1, 0
};

static const bool INV_M[16] = {
    0, 1, 0, 0,
    0, 1, 1, 1,
    0, 1, 0, 1,
    1, 0, 0, 1
};

static uint8_t RC[ROUNDS];

void SkinnyInitialization();

void SkinnyKeyExpansion(const SkinnyKey key, SkinnyRoundKey round_key);

void SkinnyEncryption(const SkinnyBlock plaintext, const SkinnyRoundKey round_key, SkinnyBlock ciphertext);

void SkinnyDecryption(const SkinnyBlock ciphertext, const SkinnyRoundKey round_key, SkinnyBlock plaintext);

void PrintBlock(const SkinnyBlock block);

int main() {
    // 生成轮常量
    SkinnyInitialization();

    // 密钥扩展
    const SkinnyKey key = SKINNY_TEST_KEY;
    SkinnyRoundKey round_key;
    SkinnyKeyExpansion(key, round_key);

    const SkinnyBlock plaintext = SKINNY_TEST_PLAINTEXT;
    SkinnyBlock ciphertext, decrypted_text;
    printf("Skinny-%d-%d\n", BLOCK_SIZE, TWEAKEY_SIZE);

    // 加密
    SkinnyEncryption(plaintext, round_key, ciphertext);
    printf("密文: ");
    PrintBlock(ciphertext);
    printf("对照: ");
    PrintBlock((SkinnyBlock)SKINNY_TEST_CIPHERTEXT);

    // 解密
    SkinnyDecryption(ciphertext, round_key, decrypted_text);
    printf("解密: ");
    PrintBlock(decrypted_text);
    printf("对照: ");
    PrintBlock((SkinnyBlock)SKINNY_TEST_PLAINTEXT);

    return 0;
}

// 生成轮常量
void SkinnyInitialization() {
    uint8_t rc = 0;
    for (int r = 0; r < ROUNDS; r++)
        RC[r] = rc = rc << 1 & 0x3F | rc >> 5 ^ rc >> 4 & 1 ^ 1;
}

// 密钥扩展
void SkinnyKeyExpansion(const SkinnyKey key, SkinnyRoundKey round_key) {
    memset(round_key, 0, sizeof(SkinnyRoundKey));

    // 初始密钥写入寄存器
    uint8_t TK[Z][16], tmp[16];
    for (int z = 0; z < Z; z++)
        for (int i = 0; i < 16; i++)
            if (S == 4)
                TK[z][i] = key[z] >> (15 - i) * 4 & 0xF;
            else if (S == 8)
                TK[z][i] = key[z * 2 + i / 8] >> (7 - i) * 8 & 0xFF;

    // 首轮轮密钥
    for (int i = 0; i < 8; i++)
        for (int z = 0; z < Z; z++)
            round_key[0][i] ^= TK[z][i];

    for (int r = 1; r < ROUNDS; r++) {
        // 线性置换层
        for (int z = 0; z < Z; z++) {
            for (int i = 0; i < 16; i++)
                tmp[i] = TK[z][P[i]];
            for (int i = 0; i < 16; i++)
                TK[z][i] = tmp[i];
        }

        // LFSR更新层(TK[1]不做)
        for (int i = 0; Z > 1 && i < 8; i++) {
            if (S == 4) {
                TK[1][i] = TK[1][i] << 1 & 0xF | TK[1][i] >> 3 ^ TK[1][i] >> 2 & 1;
                if (Z == 3)
                    TK[2][i] = TK[2][i] << 3 & 0x8 ^ TK[2][i] & 0x8 | TK[2][i] >> 1;
            } else if (S == 8) {
                TK[1][i] = TK[1][i] << 1 | TK[1][i] >> 7 ^ TK[1][i] >> 5 & 1;
                if (Z == 3)
                    TK[2][i] = TK[2][i] << 7 ^ TK[2][i] << 1 & 0x80 | TK[2][i] >> 1;
            }
        }

        // 写入轮密钥
        for (int i = 0; i < 8; i++)
            for (int z = 0; z < Z; z++)
                round_key[r][i] ^= TK[z][i];
    }
}

// 加密
void SkinnyEncryption(const SkinnyBlock plaintext, const SkinnyRoundKey round_key, SkinnyBlock ciphertext) {
    // 明文写入state
    uint8_t state[16], tmp[16] = {0};
    for (int i = 0; i < 16; i++)
        if (S == 4)
            state[i] = plaintext[0] >> (15 - i) * 4 & 0xF;
        else if (S == 8)
            state[i] = plaintext[i / 8] >> (7 - i % 8) * 8 & 0xFF;

    for (int r = 0; r < ROUNDS; r++) {
        // SubCells S盒置换
        for (int i = 0; i < 16; i++)
            if (S == 4)
                state[i] = SBOX_4[state[i]];
            else if (S == 8)
                state[i] = SBOX_8[state[i]];

        // AddConstants 轮常量加
        state[0] ^= RC[r] & 0xF;
        state[4] ^= RC[r] >> 4;
        state[8] ^= 0x2;

        // AddRoundTweakey 轮密钥加
        for (int i = 0; i < 8; i++)
            state[i] ^= round_key[r][i];

        // ShiftRows 行移位
        tmp[0] = state[7];
        memmove(state + 5, state + 4, 3 * sizeof(uint8_t));
        state[4] = tmp[0];

        memcpy(tmp, state + 10, 2 * sizeof(uint8_t));
        memcpy(state + 10, state + 8, 2 * sizeof(uint8_t));
        memcpy(state + 8, tmp, 2 * sizeof(uint8_t));

        tmp[0] = state[12];
        memmove(state + 12, state + 13, 3 * sizeof(uint8_t));
        state[15] = tmp[0];

        tmp[0] = tmp[1] = 0;

        // MixColumns 列混淆
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                for (int k = 0; k < 4; k++)
                    if (M[i * 4 + k])
                        tmp[i * 4 + j] ^= state[k * 4 + j];

        for (int i = 0; i < 16; i++)
            state[i] = tmp[i];

        memset(tmp, 0, sizeof(tmp));
    }

    // 写入结果
    memset(ciphertext, 0, sizeof(SkinnyBlock));
    for (int i = 0; i < 16; i++)
        if (S == 4)
            ciphertext[0] |= (uint64_t) state[i] << (15 - i) * 4;
        else if (S == 8)
            ciphertext[i / 8] |= (uint64_t) state[i] << (7 - i % 8) * 8;
}

// 解密
void SkinnyDecryption(const SkinnyBlock ciphertext, const SkinnyRoundKey round_key, SkinnyBlock plaintext) {
    // 密文写入state
    uint8_t state[16], tmp[16] = {0};
    for (int i = 0; i < 16; i++)
        if (S == 4)
            state[i] = ciphertext[0] >> (15 - i) * 4 & 0xF;
        else if (S == 8)
            state[i] = ciphertext[i / 8] >> (7 - i % 8) * 8 & 0xFF;

    for (int r = ROUNDS - 1; r >= 0; r--) {
        // InvMixColumns 逆列混淆
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                for (int k = 0; k < 4; k++)
                    if (INV_M[i * 4 + k])
                        tmp[i * 4 + j] ^= state[k * 4 + j];

        for (int i = 0; i < 16; i++)
            state[i] = tmp[i];

        memset(tmp, 0, sizeof(tmp));

        // InvShiftRows 逆行移位
        tmp[0] = state[4];
        memmove(state + 4, state + 5, 3 * sizeof(uint8_t));
        state[7] = tmp[0];

        memcpy(tmp, state + 10, 2 * sizeof(uint8_t));
        memcpy(state + 10, state + 8, 2 * sizeof(uint8_t));
        memcpy(state + 8, tmp, 2 * sizeof(uint8_t));

        tmp[0] = state[15];
        memmove(state + 13, state + 12, 3 * sizeof(uint8_t));
        state[12] = tmp[0];

        tmp[0] = tmp[1] = 0;

        // AddConstants 轮常量加
        state[0] ^= RC[r] & 0xF;
        state[4] ^= RC[r] >> 4;
        state[8] ^= 0x2;

        // AddRoundTweakey 轮密钥加
        for (int i = 0; i < 8; i++)
            state[i] ^= round_key[r][i];

        // InvSubCells 逆S盒置换
        for (int i = 0; i < 16; i++)
            if (S == 4)
                state[i] = INV_SBOX_4[state[i]];
            else if (S == 8)
                state[i] = INV_SBOX_8[state[i]];
    }

    // 写入结果
    memset(plaintext, 0, sizeof(SkinnyBlock));
    for (int i = 0; i < 16; i++)
        if (S == 4)
            plaintext[0] |= (uint64_t) state[i] << (15 - i) * 4;
        else if (S == 8)
            plaintext[i / 8] |= (uint64_t) state[i] << (7 - i % 8) * 8;
}

void PrintBlock(const SkinnyBlock block) {
    for (int i = 0; i < BLOCK_SIZE / 64; i++)
        printf("%016llx ", block[i]);
    printf("\n");
}
