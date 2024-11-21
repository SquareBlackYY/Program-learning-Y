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

static uint64_t Table[8][256];

// P置换
inline static uint64_t Permutation(uint64_t state) {
    uint64_t permuted = 0;
    for (int i = 0; i < 64; i++)
        if (state & 1ULL << i)
            permuted |= 1ULL << P_BOX[i];
    return permuted;
}

// 轮密钥生成
void GenerateKeySchedule(const uint16_t key[], uint64_t round_keys[]) {
    uint64_t current_key = (uint64_t) key[0] << 60
                           | (uint64_t) key[1] << 45
                           | (uint64_t) key[2] << 30
                           | (uint64_t) key[3] << 15
                           | key[4];

    for (int i = 0; i < ROUNDS; i++) {
        // 提取高64位作为轮密钥
        round_keys[i] = current_key >> 16;

        // 循环左移19位
        current_key = current_key << 19 & 0xFFFFFFFFFFFFFFFFULL |
                      current_key >> (80 - 19);

        // S盒替换最高4位
        const uint8_t sbox_input = current_key >> 76 & 0xF;
        current_key &= ~(0xFULL << 76);
        current_key |= (uint64_t) S_BOX[sbox_input] << 76;

        // 异或轮计数到密钥低5位
        current_key ^= (uint64_t) (i + 1);
    }
}

// 加密函数
void RectangleEncrypt(const uint64_t plaintext, const uint64_t round_keys[], uint64_t *ciphertext) {
    uint64_t state = plaintext;

    for (int i = 0; i < ROUNDS; i++) {
        // 与轮密钥异或
        state ^= round_keys[i];

        // S盒替换
        uint64_t sbox_state = 0;
        for (int j = 0; j < 16; j++) {
            const uint8_t nibble = state >> j * 4 & 0xF;
            sbox_state |= (uint64_t)S_BOX[nibble] << j * 4;
        }
        state = sbox_state;

        // P置换
        state = Permutation(state);
    }

    // 最后一轮异或
    state ^= round_keys[ROUNDS - 1];
    *ciphertext = state;
}

// 解密函数
void RectangleDecrypt(const uint64_t ciphertext, const uint64_t round_keys[], uint64_t *plaintext) {
    uint64_t state = ciphertext;

    // 反向最后一轮异或
    state ^= round_keys[ROUNDS - 1];

    for (int i = ROUNDS - 1; i >= 0; i--) {
        // 反向P置换
        uint64_t permuted = 0;
        for (int j = 0; j < 64; j++) {
            if (state & 1ULL << P_BOX[j]) {
                permuted |= 1ULL << j;
            }
        }
        state = permuted;

        // 反向S盒替换
        uint64_t sbox_state = 0;
        for (int j = 0; j < 16; j++) {
            const uint8_t nibble = state >> j * 4 & 0xF;
            for (int k = 0; k < 16; k++) {
                if (S_BOX[k] == nibble) {
                    sbox_state |= (uint64_t)k << j * 4;
                    break;
                }
            }
        }
        state = sbox_state;

        // 与轮密钥异或
        state ^= round_keys[i];
    }

    *plaintext = state;
}

// 初始化加速表
void InitTable() {
}

int main() {
    uint16_t key[5] = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000};
    uint64_t round_keys[ROUNDS];

    uint64_t plaintext = 0x0000000000000000ULL;
    uint64_t ciphertext, decryptedtext;

    GenerateKeySchedule(key, round_keys);
    InitTable();

    printf("明文: %016llX\n", plaintext);

    RectangleEncrypt(plaintext, round_keys, &ciphertext);
    printf("密文: %016llX\n", ciphertext);

    RectangleDecrypt(ciphertext, round_keys, &decryptedtext);
    printf("解密: %016llX\n", decryptedtext);

    return 0;
}
