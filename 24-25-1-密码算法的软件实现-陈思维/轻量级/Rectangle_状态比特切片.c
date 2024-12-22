/**
 * Rectangle 80
 * 分组长度 64
 * 密钥长度 80
 */

#include <stdio.h>
#include <stdint.h>

#define ROUNDS 25

// S 盒
static const uint8_t S_BOX[16] = {
    0x6, 0x5, 0xC, 0xA, 0x1, 0xE, 0x7, 0x9, 0xB, 0x0, 0x3, 0xD, 0x8, 0xF, 0x4, 0x2
};

static const uint8_t RC[25] = {
    0x01, 0x02, 0x04, 0x09, 0x12, 0x05, 0x0B, 0x16,
    0x0C, 0x19, 0x13, 0x07, 0x0F, 0x1F, 0x1E, 0x1C,
    0x18, 0x11, 0x03, 0x06, 0x0D, 0x1B, 0x17, 0x0E,
    0x1D
};

// S 盒
static void S(uint16_t *A) {
    uint16_t T[4];
    T[0] = ~A[1];
    T[1] = A[0] & T[0];
    T[2] = A[2] ^ A[3];
    T[3] = A[3] | T[0];
    T[3] = A[0] ^ T[3];
    T[0] = A[1] ^ A[2];
    A[0] = T[1] ^ T[2];
    T[2] = T[2] & T[3];
    T[1] = A[0] | T[0];
    A[1] = A[2] ^ T[3];
    A[2] = T[3] ^ T[1];
    A[3] = T[0] ^ T[2];
}

void GenerateKeySchedule(const uint16_t key[], uint64_t round_keys[]);
void RectangleEncrypt(const uint64_t *plaintext, const uint64_t round_keys[], uint64_t *ciphertext);

int main() {
    uint16_t key[5] = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000};
    uint64_t round_keys[ROUNDS + 1];

    printf("初始密钥: \n");
    for (int i = 0; i < 5; i++)
    {
        for (int j = 15; j >= 0; j--)
            printf("%d", key[i] >> j & 1);
        printf("\n");
    }

    uint64_t plaintext = 0x0000000000000000ULL;
    uint64_t ciphertext;

    GenerateKeySchedule(key, round_keys);

    printf("明文: \n");
    for (int i = 0; i < 4; i++)
    {
        for (int j = 15; j >= 0; j--)
            printf("%llu", plaintext >> (j + i * 16) & 1);
        printf("\n");
    }

    RectangleEncrypt(&plaintext, round_keys, &ciphertext);
    printf("密文: \n");
    for (int i = 0; i < 4; i++)
    {
        for (int j = 15; j >= 0; j--)
            printf("%llu", ciphertext >> (j + i * 16) & 1);
        printf("\n");
    }

    return 0;
}

// 轮密钥生成
void GenerateKeySchedule(const uint16_t key[], uint64_t round_keys[]) {
    uint16_t state[5];
    for (int i = 0; i < 5; i++)
        state[i] = key[i];

    // 提取轮密钥
    round_keys[0] = (uint64_t)state[3] << 48
                    | (uint64_t)state[2] << 32
                    | (uint64_t)state[1] << 16
                    | (uint64_t)state[0];

    for (int i = 1; i < ROUNDS + 1; i++) {
        // S 盒
        uint64_t tmp64 = (uint64_t)(state[0] & 0xF)
                        | (uint64_t)(state[1] & 0xF) << 16
                        | (uint64_t)(state[2] & 0xF) << 32
                        | (uint64_t)(state[3] & 0xF) << 48;
        uint16_t *p = (uint16_t *)&tmp64;
        S(p);
        state[0] = state[0] & 0xFFF0 | p[0] & 0xF;
        state[1] = state[1] & 0xFFF0 | p[1] & 0xF;
        state[2] = state[2] & 0xFFF0 | p[2] & 0xF;
        state[3] = state[3] & 0xFFF0 | p[3] & 0xF;

        // 行变换
        const uint16_t tmp16 = state[0];
        state[0] = (state[0] << 8 | state[0] >> 8) ^ state[1];
        state[1] = state[2];
        state[2] = state[3];
        state[3] = (state[3] << 12 | state[3] >> 4) ^ state[4];
        state[4] = tmp16;

        // 异或轮常数
        state[0] = state[0] & 0xFFE0 | state[0] & 0x1F ^ RC[i - 1];

        // 提取轮密钥
        round_keys[i] = (uint64_t)state[3] << 48
                        | (uint64_t)state[2] << 32
                        | (uint64_t)state[1] << 16
                        | (uint64_t)state[0];
    }
}

// 加密函数
void RectangleEncrypt(const uint64_t *plaintext, const uint64_t round_keys[], uint64_t *ciphertext) {
    *ciphertext = *plaintext;

    for (int i = 0; i < ROUNDS; i++) {
        // AddRoundKey
        *ciphertext ^= round_keys[i];

        // SubColumn
        uint16_t *X = (uint16_t *)ciphertext;
        S(X);

        // ShiftRow
        X[1] = X[1] << 1 | X[1] >> 15;
        X[2] = X[2] << 12 | X[2] >> 4;
        X[3] = X[3] << 13 | X[3] >> 3;
    }

    // AddRoundKey
    *ciphertext ^= round_keys[ROUNDS];
}