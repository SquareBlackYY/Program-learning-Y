/**
 * Gleeok-128
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define BLOCK_SIZE 128  // 分组长度
#define KEY_SIZE 256    // 密钥长度
#define KEY_ROUNDS 12   // 轮数

typedef uint64_t GleeokBlock[BLOCK_SIZE / 64];

void GleeokInitTable();

static void keyschedule(const uint64_t K00, const uint64_t K01, const uint64_t K10, const uint64_t K11, const int s, uint64_t rk[KEY_ROUNDS + 1][2]);

void GleeokKeyExpansion(const uint64_t key[], uint64_t round_key[3][KEY_ROUNDS + 1][2]);

void GleeokEncryption(const uint64_t plaintext[], const uint64_t round_key[3][KEY_ROUNDS + 1][2], uint64_t ciphertext[]);

GleeokBlock Table[16][256];

int main() {
    GleeokInitTable();

    uint64_t key[KEY_SIZE / 64] = {
        0x0001020304050607ULL, 0x08090a0b0c0d0e0fULL,
        0x1011121314151617ULL, 0x18191a1b1c1d1e1fULL};
    uint64_t round_key[3][KEY_ROUNDS + 1][2] = {0};

    // 密钥扩展
    GleeokKeyExpansion(key, &round_key);

    uint64_t plaintext[BLOCK_SIZE / 64] = {
        0x0123456789abcdefULL, 0xfedcba9876543210ULL};
    uint64_t ciphertext[BLOCK_SIZE / 64];

    // 加密
    GleeokEncryption(plaintext, &round_key, ciphertext);
    printf("密文: ");
    for (int i = 0; i < sizeof(plaintext) / sizeof(plaintext[0]); i++)
        printf("%016llx ", plaintext[i]);
    printf("\n");

    return 0;
}

void GleeokInitTable() {

}

static void keyschedule(const uint64_t K00, const uint64_t K01, const uint64_t K10, const uint64_t K11, const int s, uint64_t rk[KEY_ROUNDS + 1][2]) {
    uint64_t K0[2] = {K00, K01}, K1[2] = {K10, K11}, tmp[2];

    for (int r = 0; r < KEY_ROUNDS + 1; r++) {
        if (r % 2 == 0) {
            memcpy(tmp, K0, sizeof(uint64_t[2]));
            memset(K0, 0, sizeof(K0));
            for (int i = 0; i < BLOCK_SIZE; i++)
                K0[i / 64] |= tmp[i / 64] >> (s * i) % BLOCK_SIZE;
            memcpy(rk[r], K0, sizeof(uint64_t[2]));
        } else {

        }
    }

}

void GleeokKeyExpansion(const uint64_t key[], uint64_t round_key[3][KEY_ROUNDS + 1][2]) {
    keyschedule(key[0], key[1], key[2], key[3], 29, round_key[0]);
    keyschedule(key[2], key[3], key[0], key[1], 51, round_key[1]);
    keyschedule(key[1], key[2], key[0], key[3], 107, round_key[2]);
}

void GleeokEncryption(const uint64_t plaintext[], const uint64_t round_key[3][KEY_ROUNDS + 1][2], uint64_t ciphertext[]) {

}