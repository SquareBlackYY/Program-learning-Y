#include <stdio.h>
#include <stdint.h>

/**
 * PRESENT 80
 * 分组长度 64
 * 密钥长度 80
 */

#define ROUNDS 31

typedef struct {
    uint64_t left; // 高 64 位
    uint16_t right; // 低 16 位
} Key;

// SBox 替代盒 (4bit)
static const uint8_t S_BOX[16] = {
    0xC, 0x5, 0x6, 0xB, 0x9, 0x0, 0xA, 0xD, 0x3, 0xE, 0xF, 0x8, 0x4, 0x7, 0x1, 0x2
};
static uint8_t INV_S_BOX[16];

// PBox 置换表
static const uint8_t P_BOX[64] = {
     0, 16, 32, 48,  1, 17, 33, 49,  2, 18, 34, 50,  3, 19, 35, 51,
     4, 20, 36, 52,  5, 21, 37, 53,  6, 22, 38, 54,  7, 23, 39, 55,
     8, 24, 40, 56,  9, 25, 41, 57, 10, 26, 42, 58, 11, 27, 43, 59,
    12, 28, 44, 60, 13, 29, 45, 61, 14, 30, 46, 62, 15, 31, 47, 63
};

static uint64_t Table[8][256];

// 轮密钥生成
void GenerateKeySchedule(const Key key, uint64_t round_keys[]) {
    Key reg = key;
    for (int i = 0; i < 32; i++) {
        // 提取轮密钥
        round_keys[i] = reg.left;

        // 1. 循环左移 61 位
        const uint16_t tmp = reg.right;
        reg.right = reg.left >> 3;
        reg.left = reg.left << 61 | (uint64_t)tmp << 45 | reg.left >> 19;

        // 2. 对最左边 4 位进行 S 盒代换
        reg.left = (uint64_t)S_BOX[reg.left >> 60] << 60 | reg.left & 0xFFFFFFFFFFFFFFF;

        // 3. 将(k19, k18, k17, k16, k15)与轮计数器 i + 1 进行异或
        reg.left = reg.left & 0xFFFFFFFFFFFFFFF0 | reg.left & 0xF ^ (i + 1) >> 1;
        reg.right ^= (i + 1 & 1) << 15;
    }
}

// 加密函数
void PresentEncrypt(const uint64_t plaintext, const uint64_t round_keys[], uint64_t *ciphertext) {
    *ciphertext = plaintext;
    for (int i = 0; i < ROUNDS; i++) {
        *ciphertext ^= round_keys[i];
        *ciphertext = Table[0][*ciphertext & 0xFF]
                      | Table[1][*ciphertext >> 8 & 0xFF]
                      | Table[2][*ciphertext >> 16 & 0xFF]
                      | Table[3][*ciphertext >> 24 & 0xFF]
                      | Table[4][*ciphertext >> 32 & 0xFF]
                      | Table[5][*ciphertext >> 40 & 0xFF]
                      | Table[6][*ciphertext >> 48 & 0xFF]
                      | Table[7][*ciphertext >> 56 & 0xFF];
    }
    *ciphertext ^= round_keys[31];
}

// 解密函数
void PresentDecrypt(const uint64_t ciphertext, const uint64_t round_keys[], uint64_t *plaintext) {
    *plaintext = ciphertext ^ round_keys[31];
    for (int i = ROUNDS - 1; i >= 0 ; i--)
    {
        // pLayer
        uint64_t tmp = 0;
        for (int idx = 0; idx < 64; idx++)
            tmp |= (*plaintext >> P_BOX[idx] & 1) << idx;

        // sBoxLayer
        *plaintext = 0;
        for (int j = 0; j < 16; j++)
            *plaintext |= (uint64_t)INV_S_BOX[tmp >> 4 * j & 0xF] << 4 * j;

        // addRoundKey
        *plaintext ^= round_keys[i];
    }
}

// 初始化加速表
void InitTable() {
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            const uint8_t sbox_in = i << 4 | j;
            const uint64_t sbox_out = S_BOX[i] << 4 | S_BOX[j];

            for (int idx = 0; idx < 64; idx++)
                Table[idx / 8][sbox_in] |= (sbox_out >> idx % 8 & 1) << P_BOX[idx];
        }
    }

    for (int i = 0; i < 16; i++)
        INV_S_BOX[S_BOX[i]] = i;
}

int main() {
    Key key = {0x0000000000000000ULL, 0x0000};
    uint64_t round_keys[ROUNDS + 1];

    uint64_t plaintext = 0x0000000000000000ULL;
    uint64_t ciphertext, decryptedtext;

    GenerateKeySchedule(key, round_keys);
    InitTable();

    printf("明文: %016llX\n", plaintext);

    PresentEncrypt(plaintext, round_keys, &ciphertext);
    printf("密文: %016llX\n", ciphertext);

    PresentDecrypt(ciphertext, round_keys, &decryptedtext);
    printf("解密: %016llX\n", decryptedtext);

    return 0;
}
