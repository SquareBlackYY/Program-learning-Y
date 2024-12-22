#include <stdint.h>
#include <stdio.h>

/**
 * PRESENT 80
 * 分组长度 64
 * 密钥长度 80
 */

#define ROUNDS 25

// S 盒
static const uint8_t S_BOX[16] = {
    0x6, 0x5, 0xC, 0xA, 0x1, 0xE, 0x7, 0x9, 0xB, 0x0, 0x3, 0xD, 0x8, 0xF, 0x4, 0x2
};

// P 置换
static const uint8_t P_BOX[64] = {
    0, 16, 32, 48, 1, 17, 33, 49, 2, 18, 34, 50, 3, 19, 35, 51,
    4, 20, 36, 52, 5, 21, 37, 53, 6, 22, 38, 54, 7, 23, 39, 55,
    8, 24, 40, 56, 9, 25, 41, 57, 10, 26, 42, 58, 11, 27, 43, 59,
    12, 28, 44, 60, 13, 29, 45, 61, 14, 30, 46, 62, 15, 31, 47, 63
};

static const uint8_t RC[25] = {
    0x01, 0x02, 0x04, 0x09, 0x12, 0x05, 0x0B, 0x16,
    0x0C, 0x19, 0x13, 0x07, 0x0F, 0x1F, 0x1E, 0x1C,
    0x18, 0x11, 0x03, 0x06, 0x0D, 0x1B, 0x17, 0x0E,
    0x1D
};

// P 置换
inline static uint64_t Permutation(uint64_t state) {
    uint64_t permuted = 0;
    for (int i = 0; i < 64; i++)
        if (state & 1ULL << i)
            permuted |= 1ULL << P_BOX[i];
    return permuted;
}

// 轮密钥生成
void GenerateKeySchedule(const uint16_t key[], uint64_t round_keys[]) {
    uint16_t state[5];
    for (int i = 0; i < 5; i++)
        state[i] = key[i];

    for (int i = 0; i < ROUNDS; i++) {
        // 提取轮密钥
        round_keys[i] = (uint64_t)state[3] << 48
                        | (uint64_t)state[2] << 32
                        | (uint64_t)state[1] << 16
                        | (uint64_t)state[0];

        // 循环左移19位
        current_key = current_key << 19 & 0xFFFFFFFFFFFFFFFFULL |
                      current_key >> (80 - 19);

        // S 盒
        const uint8_t sbox_input = current_key >> 76 & 0xF;
        current_key &= ~(0xFULL << 76);
        current_key |= (uint64_t) S_BOX[sbox_input] << 76;

        // 异或轮常数
        state[0] = (state[0] & 0x1F) ^ ;
    }
}

// 加密函数
void RectangleEncrypt(const uint64_t *plaintext, const uint64_t round_keys[], uint64_t *ciphertext) {
    *ciphertext = *plaintext;

    for (int i = 0; i < ROUNDS; i++) {
        // 与轮密钥异或
        *ciphertext ^= round_keys[i];

        // S盒替换
        uint64_t sbox_state = 0;
        for (int j = 0; j < 16; j++) {
            const uint8_t nibble = *ciphertext >> j * 4 & 0xF;
            sbox_state |= (uint64_t)S_BOX[nibble] << j * 4;
        }
        *ciphertext = sbox_state;

        // P置换
        *ciphertext = Permutation(*ciphertext);
    }

    // 最后一轮异或
    *ciphertext ^= round_keys[ROUNDS - 1];
}

// 解密函数
void RectangleDecrypt(const uint64_t *ciphertext, const uint64_t round_keys[], uint64_t *plaintext) {
    *plaintext = *ciphertext;

    // 反向最后一轮异或
    *plaintext ^= round_keys[ROUNDS - 1];

    for (int i = ROUNDS - 1; i >= 0; i--) {
        // 反向P置换
        uint64_t permuted = 0;
        for (int j = 0; j < 64; j++) {
            if (*plaintext & 1ULL << P_BOX[j]) {
                permuted |= 1ULL << j;
            }
        }
        *plaintext = permuted;

        // 反向S盒替换
        uint64_t sbox_state = 0;
        for (int j = 0; j < 16; j++) {
            const uint8_t nibble = *plaintext >> j * 4 & 0xF;
            for (int k = 0; k < 16; k++) {
                if (S_BOX[k] == nibble) {
                    sbox_state |= (uint64_t)k << j * 4;
                    break;
                }
            }
        }
        *plaintext = sbox_state;

        // 与轮密钥异或
        *plaintext ^= round_keys[i];
    }
}

int main() {
    uint16_t key[5] = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000};
    uint64_t round_keys[ROUNDS];

    uint64_t plaintext = 0x0000000000000000ULL;
    uint64_t ciphertext, decryptedtext;

    GenerateKeySchedule(key, round_keys);
    InitTable();

    printf("明文: %016llX\n", plaintext);

    RectangleEncrypt(&plaintext, round_keys, &ciphertext);
    printf("密文: %016llX\n", ciphertext);

    RectangleDecrypt(&ciphertext, round_keys, &decryptedtext);
    printf("解密: %016llX\n", decryptedtext);

    return 0;
}
