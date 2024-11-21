#include <stdint.h>
#include <stdio.h>
#include <string.h>

/**
 * Simon 128 / 128
 * 分组长度 128
 * 密钥长度 128
 */

#define ROUNDS 68

static const uint64_t Z2 = 0b10101111011100000011010010011000101000010001111110010110110011Ull;

// 循环左移
inline static uint64_t S(uint64_t value, int num) {
    return (((value) << ((num) & 63)) | ((value) >> ((64 - ((num) & 63)) & 63)));
}

// 密钥生成 (m = 2)
void GenerateKeySchedule(const uint64_t *key, uint64_t round_keys[]) {
    round_keys[0] = key[1];
    round_keys[1] = key[0];
    for (int i = 2; i < ROUNDS; i++) {
        uint64_t tmp = S(round_keys[i - 1], -3);
        tmp ^= S(tmp, -1);
        round_keys[i] = ~round_keys[i - 2] ^ tmp ^ (Z2 >> (61 - ((i - 2) % 62)) & 1) ^ 3;
    }
}

// 加密函数
void SimonEncrypt(const uint64_t *plaintext, const uint64_t round_keys[], uint64_t *ciphertext) {
    uint64_t x = plaintext[0];
    uint64_t y = plaintext[1];
    for (int i = 0; i < ROUNDS; i++) {
        uint64_t tmp = x;
        x = y ^ (S(x, 1) & S(x, 8)) ^ S(x, 2) ^ round_keys[i];
        y = tmp;
    }
    ciphertext[0] = x;
    ciphertext[1] = y;
}

// 解密函数
void SimonDecrypt(const uint64_t *ciphertext, const uint64_t round_keys[], uint64_t *plaintext) {
    uint64_t x = ciphertext[1];
    uint64_t y = ciphertext[0];
    for (int i = ROUNDS - 1; i >= 0; i--) {
        uint64_t tmp = x;
        x = y ^ (S(x, 1) & S(x, 8)) ^ S(x, 2) ^ round_keys[i];
        y = tmp;
    }
    plaintext[1] = x;
    plaintext[0] = y;
}

int main() {
    uint64_t key[2] = {0x0f0e0d0c0b0a0908ULL, 0x0706050403020100ULL};
    uint64_t round_keys[ROUNDS];

    uint64_t plaintext[2] = {0x6373656420737265ULL, 0x6c6c657661727420ULL};
    uint64_t ciphertext[2], decryptedtext[2];

    // 生成轮密钥
    GenerateKeySchedule(key, round_keys);

    // 明文
    printf("明文: %016llx %016llx\n", plaintext[0], plaintext[1]);

    // 加密
    SimonEncrypt(plaintext, round_keys, ciphertext);
    printf("密文: %016llx %016llx\n", ciphertext[0], ciphertext[1]);
    
    // 解密
    SimonDecrypt(ciphertext, round_keys, decryptedtext);
    printf("解密: %016llx %016llx\n", decryptedtext[0], decryptedtext[1]);

    return 0;
}