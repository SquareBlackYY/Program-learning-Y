/**
 * Gleeok-128
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define BLOCK_SIZE 128  // 分组长度
#define KEY_SIZE 256    // 密钥长度
#define KEY_ROUNDS 12   // 轮数

static const uint8_t S3[8] = {
    0, 5, 3, 2, 6, 1, 4, 7
};

static const uint8_t S4[16] = {
    1, 0, 2, 4, 3, 8, 6, 13, 9, 10, 11, 14, 15, 12, 7, 5
};

static const uint8_t S5[32] = {
    0, 5, 10, 11, 20, 17, 22, 23, 9, 12, 3, 2, 13, 8, 15, 14,
    18, 21, 24, 27, 6, 1, 4, 7, 26, 29, 16, 19, 30, 25, 28, 31
};

static const uint64_t RC[3][KEY_ROUNDS + 1][2] = {
    {
        {0x243f6a8885a308d3ULL, 0x13198a2e03707344ULL},
        {0xa4093822299f31d0ULL, 0x082efa98ec4e6c89ULL},
        {0x452821e638d01377ULL, 0xbe5466cf34e90c6cULL},
        {0xc0ac29b7c97c50ddULL, 0x3f84d5b5b5470917ULL},
        {0x9216d5d98979fb1bULL, 0xd1310ba698dfb5acULL},
        {0x2ffd72dbd01adfb7ULL, 0xb8e1afed6a267e96ULL},
        {0xba7c9045f12c7f99ULL, 0x24a19947b3916cf7ULL},
        {0x0801f2e2858efc16ULL, 0x636920d871574e69ULL},
        {0xa458fea3f4933d7eULL, 0x0d95748f728eb658ULL},
        {0x718bcd5882154aeeULL, 0x7b54a41dc25a59b5ULL},
        {0x9c30d5392af26013ULL, 0xc5d1b023286085f0ULL},
        {0xca417918b8db38efULL, 0x8e79dcb0603a180eULL},
        {0x6c9e0e8bb01e8a3eULL, 0xd71577c1bd314b27ULL}
    },
    {
        {0x78af2fda55605c60ULL, 0xe65525f3aa55ab94ULL},
        {0x5748986263e81440ULL, 0x55ca396a2aab10b6ULL},
        {0xb4cc5c341141e8ceULL, 0xa15486af7c72e993ULL},
        {0xb3ee1411636fbc2aULL, 0x2ba9c55d741831f6ULL},
        {0xce5c3e169b87931eULL, 0xafd6ba336c24cf5cULL},
        {0x7a32538128958677ULL, 0x3b8f48986b4bb9afULL},
        {0xc4bfe81b66282193ULL, 0x61d809ccfb21a991ULL},
        {0x487cac605dec8032ULL, 0xef845d5de98575b1ULL},
        {0xdc262302eb651b88ULL, 0x23893e81d396acc5ULL},
        {0x0f6d6ff383f44239ULL, 0x2e0b4482a4842004ULL},
        {0x69c8f04a9e1f9b5eULL, 0x21c66842f6e96c9aULL},
        {0x670c9c61abd388f0ULL, 0x6a51a0d2d8542f68ULL},
        {0x960fa728ab5133a3ULL, 0x6eef0b6c137a3be4ULL}
    },
    {
        {0xba3bf0507efb2a98ULL, 0xa1f1651d39af0176ULL},
        {0x66ca593e82430e88ULL, 0x8cee8619456f9fb4ULL},
        {0x7d84a5c33b8b5ebeULL, 0xe06f75d885c12073ULL},
        {0x401a449f56c16aa6ULL, 0x4ed3aa62363f7706ULL},
        {0x1bfedf72429b023dULL, 0x37d0d724d00a1248ULL},
        {0xdb0fead349f1c09bULL, 0x075372c980991b7bULL},
        {0x25d479d8f6e8def7ULL, 0xe3fe501ab6794c3bULL},
        {0x976ce0bd04c006baULL, 0xc1a94fb6409f60c4ULL},
        {0x5e5c9ec2196a2463ULL, 0x68fb6faf3e6c53b5ULL},
        {0x1339b2eb3b52ec6fULL, 0x6dfc511f9b30952cULL},
        {0xcc814544af5ebd09ULL, 0xbee3d004de334afdULL},
        {0x660f2807192e4bb3ULL, 0xc0cba85745c8740fULL},
        {0xd20b5f39b9d3fbdbULL, 0x5579c0bd1a60320aULL}
    }
};

void GleeokInitTable();

static void lookup_table(uint64_t text[], int table_num);

static void keyschedule(uint64_t K00, uint64_t K01, uint64_t K10, uint64_t K11, int s, uint64_t rk[KEY_ROUNDS + 1][2]);

void GleeokKeyExpansion(const uint64_t key[], uint64_t round_key[3][KEY_ROUNDS + 1][2]);

static void branch0(uint64_t text[], const uint64_t branch_key[], const uint64_t rc[], bool last);

static void branch1(uint64_t text[], const uint64_t branch_key[], const uint64_t rc[], bool last);

static void branch2(uint64_t text[], const uint64_t branch_key[], const uint64_t rc[], bool last);

void GleeokEncryption(const uint64_t plaintext[], const uint64_t round_key[3][KEY_ROUNDS + 1][2],
                      uint64_t ciphertext[]);

uint64_t Table[3][16][256][2];

int main() {
    // 加速表初始化
    GleeokInitTable();

    uint64_t key[KEY_SIZE / 64] = {
        0x0001020304050607ULL, 0x08090a0b0c0d0e0fULL,
        0x1011121314151617ULL, 0x18191a1b1c1d1e1fULL
    };
    uint64_t round_key[3][KEY_ROUNDS + 1][2] = {0};

    // 密钥扩展
    GleeokKeyExpansion(key, round_key);

    uint64_t plaintext[BLOCK_SIZE / 64] = {
        0x0123456789abcdefULL, 0xfedcba9876543210ULL
    };
    printf("明文: ");
    for (int i = 0; i < sizeof(plaintext) / sizeof(plaintext[0]); i++)
        printf("%016llx ", plaintext[i]);
    printf("\n");
    uint64_t ciphertext[BLOCK_SIZE / 64];

    // 加密
    GleeokEncryption(plaintext, round_key, ciphertext);
    printf("密文: ");
    for (int i = 0; i < sizeof(ciphertext) / sizeof(ciphertext[0]); i++)
        printf("%016llx ", ciphertext[i]);
    printf("\n");

    return 0;
}

// 加速表初始化
void GleeokInitTable() {
    // The parameters of theta
    const int t[3][3] = {
        {12, 31, 86},
        {4, 23, 78},
        {7, 15, 23}
    };
    // The parameters of pi
    const int p[3] = {117, 117, 11};
    uint64_t temp[2];

    for (int branch_num = 0; branch_num < 3; branch_num++) {
        for (int i = 0; i < 16; i++) {
            for (int sbox_in = 0; sbox_in < 256; sbox_in++) {
                // S盒置换 S-box
                if (branch_num != 2) {
                    Table[branch_num][i][sbox_in][i / 8] =
                            (uint64_t) (S3[sbox_in >> 5] << 5 | S5[sbox_in & 0x1F]) << (56 - i % 8 * 8);
                } else {
                    Table[branch_num][i][sbox_in][i / 8] =
                            (uint64_t) (S4[sbox_in >> 4] << 4 | S4[sbox_in & 0xF]) << (56 - i % 8 * 8);
                }

                // 3 XOR theta
                temp[0] = (Table[branch_num][i][sbox_in][t[branch_num][0] / 64] << (t[branch_num][0] % 64)
                           | Table[branch_num][i][sbox_in][!(t[branch_num][0] / 64)] >> (64 - t[branch_num][0] % 64))
                          ^ (Table[branch_num][i][sbox_in][t[branch_num][1] / 64] << (t[branch_num][1] % 64)
                             | Table[branch_num][i][sbox_in][!(t[branch_num][1] / 64)] >> (64 - t[branch_num][1] % 64))
                          ^ (Table[branch_num][i][sbox_in][t[branch_num][2] / 64] << (t[branch_num][2] % 64)
                             | Table[branch_num][i][sbox_in][!(t[branch_num][2] / 64)] >> (64 - t[branch_num][2] % 64));
                temp[1] = (Table[branch_num][i][sbox_in][!(t[branch_num][0] / 64)] << (t[branch_num][0] % 64)
                           | Table[branch_num][i][sbox_in][t[branch_num][0] / 64] >> (64 - t[branch_num][0] % 64))
                          ^ (Table[branch_num][i][sbox_in][!(t[branch_num][1] / 64)] << (t[branch_num][1] % 64)
                             | Table[branch_num][i][sbox_in][t[branch_num][1] / 64] >> (64 - t[branch_num][1] % 64))
                          ^ (Table[branch_num][i][sbox_in][!(t[branch_num][2] / 64)] << (t[branch_num][2] % 64)
                             | Table[branch_num][i][sbox_in][t[branch_num][2] / 64] >> (64 - t[branch_num][2] % 64));

                // Permutation pi
                memset(Table[branch_num][i][sbox_in], 0, sizeof(uint64_t[2]));
                for (int j = 0; j < 128; j++) {
                    Table[branch_num][i][sbox_in][j / 64] |=
                            (temp[p[branch_num] * j % 128 / 64] >> (63 - p[branch_num] * j % 64) & 1ULL)
                            << (63 - j % 64);
                }
            }
        }
    }
}

// 查表
static void lookup_table(uint64_t text[], const int table_num) {
    uint64_t temp[2];
    memcpy(temp, text, sizeof(uint64_t[2]));
    text[0] = Table[table_num][0][temp[0] >> 56][0]
              ^ Table[table_num][1][(temp[0] >> 48) & 0xFF][0]
              ^ Table[table_num][2][(temp[0] >> 40) & 0xFF][0]
              ^ Table[table_num][3][(temp[0] >> 32) & 0xFF][0]
              ^ Table[table_num][4][(temp[0] >> 24) & 0xFF][0]
              ^ Table[table_num][5][(temp[0] >> 16) & 0xFF][0]
              ^ Table[table_num][6][(temp[0] >> 8) & 0xFF][0]
              ^ Table[table_num][7][temp[0] & 0xFF][0]
              ^ Table[table_num][8][temp[1] >> 56][0]
              ^ Table[table_num][9][(temp[1] >> 48) & 0xFF][0]
              ^ Table[table_num][10][(temp[1] >> 40) & 0xFF][0]
              ^ Table[table_num][11][(temp[1] >> 32) & 0xFF][0]
              ^ Table[table_num][12][(temp[1] >> 24) & 0xFF][0]
              ^ Table[table_num][13][(temp[1] >> 16) & 0xFF][0]
              ^ Table[table_num][14][(temp[1] >> 8) & 0xFF][0]
              ^ Table[table_num][15][temp[1] & 0xFF][0];

    text[1] = Table[table_num][0][temp[0] >> 56][1]
              ^ Table[table_num][1][(temp[0] >> 48) & 0xFF][1]
              ^ Table[table_num][2][(temp[0] >> 40) & 0xFF][1]
              ^ Table[table_num][3][(temp[0] >> 32) & 0xFF][1]
              ^ Table[table_num][4][(temp[0] >> 24) & 0xFF][1]
              ^ Table[table_num][5][(temp[0] >> 16) & 0xFF][1]
              ^ Table[table_num][6][(temp[0] >> 8) & 0xFF][1]
              ^ Table[table_num][7][temp[0] & 0xFF][1]
              ^ Table[table_num][8][temp[1] >> 56][1]
              ^ Table[table_num][9][(temp[1] >> 48) & 0xFF][1]
              ^ Table[table_num][10][(temp[1] >> 40) & 0xFF][1]
              ^ Table[table_num][11][(temp[1] >> 32) & 0xFF][1]
              ^ Table[table_num][12][(temp[1] >> 24) & 0xFF][1]
              ^ Table[table_num][13][(temp[1] >> 16) & 0xFF][1]
              ^ Table[table_num][14][(temp[1] >> 8) & 0xFF][1]
              ^ Table[table_num][15][temp[1] & 0xFF][1];
}

// 轮密钥生成
static void keyschedule(const uint64_t K00, const uint64_t K01, const uint64_t K10, const uint64_t K11, const int s,
                        uint64_t rk[KEY_ROUNDS + 1][2]) {
    uint64_t K0[2] = {K00, K01}, K1[2] = {K10, K11}, tmp[2];

    for (int r = 0; r < KEY_ROUNDS + 1; r++) {
        if (r % 2 == 0) {
            memcpy(tmp, K0, sizeof(uint64_t[2]));
            memset(K0, 0, sizeof(uint64_t[2]));
            for (int i = 0; i < BLOCK_SIZE; i++) {
                K0[i / 64] |= ((tmp[(s * i) % BLOCK_SIZE / 64] >> (63 - (s * i) % 64)) & 1ULL) << (63 - i % 64);
            }
            memcpy(rk[r], K0, sizeof(uint64_t[2]));
        } else {
            memcpy(tmp, K1, sizeof(uint64_t[2]));
            memset(K1, 0, sizeof(uint64_t[2]));
            for (int i = 0; i < BLOCK_SIZE; i++) {
                K1[i / 64] |= ((tmp[(s * i) % BLOCK_SIZE / 64] >> (63 - (s * i) % 64)) & 1ULL) << (63 - i % 64);
            }
            memcpy(rk[r], K1, sizeof(uint64_t[2]));
        }
    }
}

// 密钥扩展
void GleeokKeyExpansion(const uint64_t key[], uint64_t round_key[3][KEY_ROUNDS + 1][2]) {
    keyschedule(key[0], key[1], key[2], key[3], 29, round_key[0]);
    keyschedule(key[2], key[3], key[0], key[1], 51, round_key[1]);
    keyschedule(key[1], key[2], key[0], key[3], 107, round_key[2]);
}

// 分支函数0
static void branch0(uint64_t text[], const uint64_t branch_key[], const uint64_t rc[], const bool last) {
    if (!last) {
        // 线性层查表 S-box, 3 XOR, Permutation
        lookup_table(text, 0);
    } else {
        // S盒置换 S-box
        uint64_t temp[2] = {0};
        for (int i = 0; i < 8; i++) {
            temp[0] |= (uint64_t) S3[text[0] >> (61 - i * 8) & 0x7] << (61 - i * 8);
            temp[0] |= (uint64_t) S5[text[0] >> (56 - i * 8) & 0x1F] << (56 - i * 8);
            temp[1] |= (uint64_t) S3[text[1] >> (61 - i * 8) & 0x7] << (61 - i * 8);
            temp[1] |= (uint64_t) S5[text[1] >> (56 - i * 8) & 0x1F] << (56 - i * 8);
        }
        memcpy(text, temp, sizeof(uint64_t[2]));
    }

    // 轮密钥加 Key Addition
    text[0] ^= branch_key[0];
    text[1] ^= branch_key[1];

    // 轮常量加 Constant Addition
    text[0] ^= rc[0];
    text[1] ^= rc[1];
}

// 分支函数1
static void branch1(uint64_t text[], const uint64_t branch_key[], const uint64_t rc[], const bool last) {
    if (!last) {
        // 线性层查表 S-box, 3 XOR, Permutation
        lookup_table(text, 1);
    } else {
        // S盒置换 S-box
        uint64_t temp[2] = {0};
        for (int i = 0; i < 8; i++) {
            temp[0] |= (uint64_t) S3[text[0] >> (61 - i * 8) & 0x7] << (61 - i * 8);
            temp[0] |= (uint64_t) S5[text[0] >> (56 - i * 8) & 0x1F] << (56 - i * 8);
            temp[1] |= (uint64_t) S3[text[1] >> (61 - i * 8) & 0x7] << (61 - i * 8);
            temp[1] |= (uint64_t) S5[text[1] >> (56 - i * 8) & 0x1F] << (56 - i * 8);
        }
        memcpy(text, temp, sizeof(uint64_t[2]));
    }

    // 轮密钥加 Key Addition
    text[0] ^= branch_key[0];
    text[1] ^= branch_key[1];

    // 轮常量加 Constant Addition
    text[0] ^= rc[0];
    text[1] ^= rc[1];
}

// 分支函数2
static void branch2(uint64_t text[], const uint64_t branch_key[], const uint64_t rc[], const bool last) {
    if (!last) {
        // 线性层查表 S-box, 3 XOR, Permutation
        lookup_table(text, 2);
    } else {
        // S盒置换 S-box
        uint64_t temp[2] = {0};
        for (int i = 0; i < 16; i++) {
            temp[0] |= (uint64_t) S4[text[0] >> (60 - i * 4) & 0xF] << (60 - i * 4);
            temp[1] |= (uint64_t) S4[text[1] >> (60 - i * 4) & 0xF] << (60 - i * 4);
        }
        memcpy(text, temp, sizeof(uint64_t[2]));
    }

    // 轮密钥加 Key Addition
    text[0] ^= branch_key[0];
    text[1] ^= branch_key[1];

    // 轮常量加 Constant Addition
    text[0] ^= rc[0];
    text[1] ^= rc[1];
}

// 加密函数
void GleeokEncryption(const uint64_t plaintext[], const uint64_t round_key[3][KEY_ROUNDS + 1][2],
                      uint64_t ciphertext[]) {
    uint64_t b[3][2] = {0};

    // 异或白话密钥
    b[0][0] = plaintext[0] ^ round_key[0][0][0];
    b[0][1] = plaintext[1] ^ round_key[0][0][1];
    b[1][0] = plaintext[0] ^ round_key[1][0][0];
    b[1][1] = plaintext[1] ^ round_key[1][0][1];
    b[2][0] = plaintext[0] ^ round_key[2][0][0];
    b[2][1] = plaintext[1] ^ round_key[2][0][1];

    // 轮函数
    for (int r = 1; r < KEY_ROUNDS; r++) {
        branch0(b[0], round_key[0][r], RC[0][r], false);
        branch1(b[1], round_key[1][r], RC[1][r], false);
        branch2(b[2], round_key[2][r], RC[2][r], false);
    }

    // 最后一轮
    branch0(b[0], round_key[0][KEY_ROUNDS], RC[0][KEY_ROUNDS], true);
    branch1(b[1], round_key[1][KEY_ROUNDS], RC[1][KEY_ROUNDS], true);
    branch2(b[2], round_key[2][KEY_ROUNDS], RC[2][KEY_ROUNDS], true);

    // 结果输出
    ciphertext[0] = b[0][0] ^ b[1][0] ^ b[2][0];
    ciphertext[1] = b[0][1] ^ b[1][1] ^ b[2][1];
}
