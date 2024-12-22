/**
 * Ascon-128
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define KEY_SIZE 128                                        // 密钥长度
#define NONCE_SIZE 128                                      // 随机数长度
#define TAG_SIZE 128                                        // 标签长度
#define DATA_BLOCK 64                                       // 分组长度
#define ROUNDS_A 12                                         // p 变换轮数 A
#define ROUNDS_B 6                                          // p 变换轮数 B
#define ASCON_IV 0x80400C0600000000                         // 初始 IV
#define RR(state, num) (state >> num | state << (64 - num)) // 64位循环右移

// p 变换常量
static const uint64_t C_R[12] = {
    0xf0ULL, 0xe1ULL, 0xd2ULL, 0xc3ULL, 0xb4ULL, 0xa5ULL,
    0x96ULL, 0x87ULL, 0x78ULL, 0x69ULL, 0x5aULL, 0x4bULL
};

void Sbox(uint64_t x[]){
    uint64_t t0,t1;
    x[0] ^= x[4];
    x[4] ^= x[3];
    x[2] ^= x[1];
    t0 = x[0] & (~x[4]);
    t1 = x[2] & (~x[1]);
    x[0] ^= t1;
    t1 = x[4] & (~x[3]);
    x[2] ^= t1;
    t1 = x[1] & (~x[0]);
    x[4] ^= t1;
    t1 = x[3] & (~x[2]);
    x[1] ^= t1;
    x[3] ^= t0;
    x[1] ^= x[0];
    x[3] ^= x[2];
    x[0] ^= x[4];
    x[2] = ~x[2];
}

// p 变换
static void Permutation(uint64_t state[], const int num) {
    for (int i = 0; i < num; i++) {
        // Round constant addtion p_C 轮常数加
        state[2] ^= C_R[i + 12 - num];

        // Subsititution layer p_S with 5-bit sbox 5位S盒置换层
        // uint64_t tmp[5];
        // memcpy(tmp, state, sizeof(uint64_t) * 5);
        // state[0] = tmp[4] & tmp[1] ^ tmp[3] ^ tmp[2] & tmp[1] ^ tmp[2] ^ tmp[1] & tmp[0] ^ tmp[1] ^ tmp[0];
        // state[1] = tmp[4] ^ tmp[3] & tmp[2] ^ tmp[3] & tmp[1] ^ tmp[3] ^ tmp[2] & tmp[1] ^ tmp[2] ^ tmp[1] ^ tmp[0];
        // state[2] = tmp[4] & tmp[3] ^ tmp[4] ^ tmp[2] ^ tmp[1] ^ ~0ULL;
        // state[3] = tmp[4] & tmp[0] ^ tmp[4] ^ tmp[3] & tmp[0] ^ tmp[3] ^ tmp[2] ^ tmp[1] & tmp[0];
        // state[4] = tmp[4] & tmp[1] ^ tmp[4] ^ tmp[3] ^ tmp[1] & tmp[0] ^ tmp[1];
        Sbox(state);
        // Linear layer with 64-bit diffusion functions 64位线性层
        state[0] ^= RR(state[0], 19) ^ RR(state[0], 28);
        state[1] ^= RR(state[1], 61) ^ RR(state[1], 39);
        state[2] ^= RR(state[2], 1) ^ RR(state[2], 6);
        state[3] ^= RR(state[3], 10) ^ RR(state[3], 17);
        state[4] ^= RR(state[4], 7) ^ RR(state[4], 41);
    }
}

void AsconEncrypt(const uint64_t K[], const uint64_t N[], const uint64_t A[], const uint64_t P[], uint64_t C[], uint64_t T[], const size_t num_blocks_A, const size_t num_blocks_P);
bool AsconDecrypt(const uint64_t K[], const uint64_t N[], const uint64_t A[], const uint64_t C[], const uint64_t T[], uint64_t P[], const size_t num_blocks_A, const size_t num_blocks_C);

int main() {
    uint64_t K[KEY_SIZE / 64] = {0x0001020304050607ULL, 0x08090A0B0C0D0E0FULL};
    uint64_t N[NONCE_SIZE / 64] = {0x0001020304050607ULL, 0x08090A0B0C0D0E0FULL};
    uint64_t A[2] = {0x0001020304050607ULL, 0x0880000000000000ULL};
    uint64_t P[3] = {0x0001020304050607ULL, 0x08090A0B0C0D0E0FULL, 0x1011121314151680ULL};
    uint64_t C[3], T[TAG_SIZE / 64];

    // 加密及认证
    AsconEncrypt(K, N, A, P, C, T, sizeof(A) / sizeof(A[0]), sizeof(P) / sizeof(P[0]));

    printf("密文:\n");
    for (int i = 0; i < sizeof(C) / sizeof(C[0]); i++)
        printf("%016llX ", C[i]);
    printf("\n");

    printf("标签:\n%016llX %016llX\n", T[0], T[1]);

    // 解密及验证
    if (AsconDecrypt(K, N, A, C, T, P, sizeof(A) / sizeof(A[0]), sizeof(C) / sizeof(C[0])))
        printf("标签验证通过。\n");

    printf("明文:\n");
    for (int i = 0; i < sizeof(P) / sizeof(P[0]); i++)
        printf("%016llX ", P[i]);
    printf("\n");

    return 0;
}

/**
 * Ascon 加密及认证函数
 * @param K 密钥
 * @param N 随机数
 * @param A 关联数据
 * @param P 明文
 * @param C 加密后的密文
 * @param T 标签
 */
void AsconEncrypt(const uint64_t K[], const uint64_t N[], const uint64_t A[], const uint64_t P[], uint64_t C[], uint64_t T[], const size_t num_blocks_A, const size_t num_blocks_P) {
    uint64_t state[5];

    // Initialization 初始化
    state[0] = ASCON_IV;
    state[1] = K[0];
    state[2] = K[1];
    state[3] = N[0];
    state[4] = N[1];
    Permutation(state, ROUNDS_A);
    state[3] ^= K[0];
    state[4] ^= K[1];

    // Associated Data 关联数据
    for (int i = 0; i < num_blocks_A; i++) {
        state[0] ^= A[i];
        Permutation(state, ROUNDS_B);
    }
    state[4] ^= 1ULL;

    // Plaintext 加密明文
    C[0] = state[0] ^= P[0];
    for (int i = 1; i < num_blocks_P; i++) {
        Permutation(state, ROUNDS_B);
        C[i] = state[0] ^= P[i];
    }

    // Finalization 生成标签
    state[1] ^= K[0];
    state[2] ^= K[1];
    Permutation(state, ROUNDS_A);
    T[0] = state[3] ^ K[0];
    T[1] = state[4] ^ K[1];
}

/**
 * Ascon 解密及验证函数
 * @param K 密钥
 * @param N 随机数
 * @param A 关联数据
 * @param C 密文
 * @param T 标签
 * @param P 解密后的明文
 * @return 验证标签合法性
 */
bool AsconDecrypt(const uint64_t K[], const uint64_t N[], const uint64_t A[], const uint64_t C[], const uint64_t T[], uint64_t P[], const size_t num_blocks_A, const size_t num_blocks_C) {
    uint64_t state[5], T_prime[TAG_SIZE / 64];

    // Initialization 初始化
    state[0] = ASCON_IV;
    state[1] = K[0];
    state[2] = K[1];
    state[3] = N[0];
    state[4] = N[1];
    Permutation(state, ROUNDS_A);
    state[3] ^= K[0];
    state[4] ^= K[1];

    // Associated Data 关联数据
    for (int i = 0; i < num_blocks_A; i++) {
        state[0] ^= A[i];
        Permutation(state, ROUNDS_B);
    }
    state[4] ^= 1ULL;

    // Ciphertext 解密密文
    P[0] = state[0] ^ C[0];
    state[0] = C[0];
    for (int i = 1; i < num_blocks_C; i++) {
        Permutation(state, ROUNDS_B);
        P[i] = state[0] ^ C[i];
        state[0] = C[i];
    }

    // Finalization 生成标签
    state[1] ^= K[0];
    state[2] ^= K[1];
    Permutation(state, ROUNDS_A);
    T_prime[0] = state[3] ^ K[0];
    T_prime[1] = state[4] ^ K[1];

    // 验证标签
    return T[0] == T_prime[0] && T[1] == T_prime[1];
}
