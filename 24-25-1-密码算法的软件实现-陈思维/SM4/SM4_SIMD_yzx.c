#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <emmintrin.h>
#include <immintrin.h>

#define BLOCK_SIZE 1048576      // 2^20

// SM4 constants
static const uint32_t FK[4] = {
    0xa3b1bac6, 0x56aa3350, 0x677d9197, 0xb27022dc
};

// CK[ij] = (4 * i + j) * 7 % 256
static const uint32_t CK[32] = {
    0x00070e15, 0x1c232a31, 0x383f464d, 0x545b6269,
    0x70777e85, 0x8c939aa1, 0xa8afb6bd, 0xc4cbd2d9,
    0xe0e7eef5, 0xfc030a11, 0x181f262d, 0x343b4249,
    0x50575e65, 0x6c737a81, 0x888f969d, 0xa4abb2b9,
    0xc0c7ced5, 0xdce3eaf1, 0xf8ff060d, 0x141b2229,
    0x30373e45, 0x4c535a61, 0x686f767d, 0x848b9299,
    0xa0a7aeb5, 0xbcc3cad1, 0xd8dfe6ed, 0xf4fb0209,
    0x10171e25, 0x2c333a41, 0x484f565d, 0x646b7279
};

// S-box
static const uint8_t Sbox[256] = {
    0xd6, 0x90, 0xe9, 0xfe, 0xcc, 0xe1, 0x3d, 0xb7, 0x16, 0xb6, 0x14, 0xc2, 0x28, 0xfb, 0x2c, 0x05,
    0x2b, 0x67, 0x9a, 0x76, 0x2a, 0xbe, 0x04, 0xc3, 0xaa, 0x44, 0x13, 0x26, 0x49, 0x86, 0x06, 0x99,
    0x9c, 0x42, 0x50, 0xf4, 0x91, 0xef, 0x98, 0x7a, 0x33, 0x54, 0x0b, 0x43, 0xed, 0xcf, 0xac, 0x62,
    0xe4, 0xb3, 0x1c, 0xa9, 0xc9, 0x08, 0xe8, 0x95, 0x80, 0xdf, 0x94, 0xfa, 0x75, 0x8f, 0x3f, 0xa6,
    0x47, 0x07, 0xa7, 0xfc, 0xf3, 0x73, 0x17, 0xba, 0x83, 0x59, 0x3c, 0x19, 0xe6, 0x85, 0x4f, 0xa8,
    0x68, 0x6b, 0x81, 0xb2, 0x71, 0x64, 0xda, 0x8b, 0xf8, 0xeb, 0x0f, 0x4b, 0x70, 0x56, 0x9d, 0x35,
    0x1e, 0x24, 0x0e, 0x5e, 0x63, 0x58, 0xd1, 0xa2, 0x25, 0x22, 0x7c, 0x3b, 0x01, 0x21, 0x78, 0x87,
    0xd4, 0x00, 0x46, 0x57, 0x9f, 0xd3, 0x27, 0x52, 0x4c, 0x36, 0x02, 0xe7, 0xa0, 0xc4, 0xc8, 0x9e,
    0xea, 0xbf, 0x8a, 0xd2, 0x40, 0xc7, 0x38, 0xb5, 0xa3, 0xf7, 0xf2, 0xce, 0xf9, 0x61, 0x15, 0xa1,
    0xe0, 0xae, 0x5d, 0xa4, 0x9b, 0x34, 0x1a, 0x55, 0xad, 0x93, 0x32, 0x30, 0xf5, 0x8c, 0xb1, 0xe3,
    0x1d, 0xf6, 0xe2, 0x2e, 0x82, 0x66, 0xca, 0x60, 0xc0, 0x29, 0x23, 0xab, 0x0d, 0x53, 0x4e, 0x6f,
    0xd5, 0xdb, 0x37, 0x45, 0xde, 0xfd, 0x8e, 0x2f, 0x03, 0xff, 0x6a, 0x72, 0x6d, 0x6c, 0x5b, 0x51,
    0x8d, 0x1b, 0xaf, 0x92, 0xbb, 0xdd, 0xbc, 0x7f, 0x11, 0xd9, 0x5c, 0x41, 0x1f, 0x10, 0x5a, 0xd8,
    0x0a, 0xc1, 0x31, 0x88, 0xa5, 0xcd, 0x7b, 0xbd, 0x2d, 0x74, 0xd0, 0x12, 0xb8, 0xe5, 0xb4, 0xb0,
    0x89, 0x69, 0x97, 0x4a, 0x0c, 0x96, 0x77, 0x7e, 0x65, 0xb9, 0xf1, 0x09, 0xc5, 0x6e, 0xc6, 0x84,
    0x18, 0xf0, 0x7d, 0xec, 0x3a, 0xdc, 0x4d, 0x20, 0x79, 0xee, 0x5f, 0x3e, 0xd7, 0xcb, 0x39, 0x48
};

static uint32_t tau(uint32_t A);            // Function to perform the non-linear transformation tau
static uint32_t L(uint32_t B);              // Function to perform the linear transformation L for encryption
static uint32_t L_prime(uint32_t B);        // Function to perform the linear transformation L' for key expansion
static uint32_t T(uint32_t A);              // Function to perform the T transformation for encryption
static uint32_t T_prime(uint32_t A);        // Function to perform the T' transformation for key expansion
void SM4_KeySchedule(const uint32_t MK[4], __m128i rk[32]);         // Key expansion function
void SM4_Encrypt(uint32_t plaintext[16], __m128i rk[32], uint32_t ciphertext[16]);          // Encryption function
void SM4_Decrypt(uint32_t ciphertext[16], __m128i rk[32], uint32_t decryptedtext[16]);      // Decryption function
void init_table();                                  // Init table
void print_block(const uint32_t block[16]);         // Utility function to print 128-bit blocks

uint32_t Table[4][256];     // (Sbox + L)Table

int main() {
    uint32_t MK[4] = {0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210};
    uint32_t plaintext[16] = {0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210, 0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210, 0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210, 0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210};
    uint32_t ciphertext[16];
    uint32_t decryptedtext[16];
    __m128i rk[32];

    // Key expansion
    SM4_KeySchedule(MK, rk);

    // Init table
    init_table();

    // for (int i = 0; i < 256; ++i)
    //     printf("%08x ", Table[0][i]);
    // printf("\n");

    printf("Plaintext:\t");
    print_block(plaintext);

    // Encryption
    clock_t start = clock();
    for (int i = 0; i < BLOCK_SIZE; ++i) {
        SM4_Encrypt(plaintext, rk, ciphertext);
    }
    clock_t end = clock();

    int data = 4 * 16 * BLOCK_SIZE / 1024 / 1024;
    printf("加密 %d MB 数据量，速度 : %lf Mbps\n", data, 8 * data / ((double)(end - start) / CLOCKS_PER_SEC));

    printf("Ciphertext:\t");
    print_block(ciphertext);

    // Decryption
    SM4_Decrypt(ciphertext, rk, decryptedtext);
    printf("Decrypted:\t");
    print_block(decryptedtext);

    return 0;
}

// Function to perform the non-linear transformation tau
static uint32_t tau(uint32_t A) {
    uint8_t a0 = Sbox[(A >> 24) & 0xFF];
    uint8_t a1 = Sbox[(A >> 16) & 0xFF];
    uint8_t a2 = Sbox[(A >> 8) & 0xFF];
    uint8_t a3 = Sbox[A & 0xFF];
    return ((uint32_t)a0 << 24) | ((uint32_t)a1 << 16) | ((uint32_t)a2 << 8) | a3;
}

// Function to perform the linear transformation L for encryption
static uint32_t L(uint32_t B) {
    return B ^ (B << 2 | B >> 30) ^ (B << 10 | B >> 22) ^ (B << 18 | B >> 14) ^ (B << 24 | B >> 8);
}

// Function to perform the linear transformation L' for key expansion
static uint32_t L_prime(uint32_t B) {
    return B ^ (B << 13 | B >> 19) ^ (B << 23 | B >> 9);
}

// Function to perform the T transformation for encryption
static uint32_t T(uint32_t A) {
    return L(tau(A));
}

// Function to perform the T' transformation for key expansion
static uint32_t T_prime(uint32_t A) {
    return L_prime(tau(A));
}

// Key expansion function
void SM4_KeySchedule(const uint32_t MK[4], __m128i rk[32]) {
    uint32_t K[36];
    K[0] = MK[0] ^ FK[0];
    K[1] = MK[1] ^ FK[1];
    K[2] = MK[2] ^ FK[2];
    K[3] = MK[3] ^ FK[3];
    for (int i = 0; i < 32; i++) {
        K[i + 4] = K[i] ^ T_prime(K[i + 1] ^ K[i + 2] ^ K[i + 3] ^ CK[i]);
        rk[i] = _mm_setr_epi32(K[i + 4], K[i + 4], K[i + 4], K[i + 4]);
    }
}

// Encryption function
void SM4_Encrypt(uint32_t plaintext[16], __m128i rk[32], uint32_t ciphertext[16]) {
    __m128i X[36], temp[4], Y[4];
    for (int i = 0; i < 4; ++i) {
        __m128i vindex = _mm_setr_epi32(i, i + 4, i + 8, i + 12);
        X[i] = _mm_i32gather_epi32((int*)plaintext, vindex, 4);
    }

    __m128i mask = _mm_setr_epi32(0xff, 0xff, 0xff, 0xff);
    for (int i = 0; i < 32; i++) {
        X[i + 4] = _mm_xor_si128(_mm_xor_si128(_mm_xor_si128(X[i + 1], X[i + 2]), X[i + 3]), rk[i]);

        temp[0] = _mm_and_si128(_mm_srli_epi32(X[i + 4], 24), mask);
        Y[0] = _mm_i32gather_epi32((int*)Table[0], temp[0], 4);

        temp[1] = _mm_and_si128(_mm_srli_epi32(X[i + 4], 16), mask);
        Y[1] = _mm_i32gather_epi32((int*)Table[1], temp[1], 4);

        temp[2] = _mm_and_si128(_mm_srli_epi32(X[i + 4], 8), mask);
        Y[2] = _mm_i32gather_epi32((int*)Table[2], temp[2], 4);

        temp[3] = _mm_and_si128(X[i + 4], mask);
        Y[3] = _mm_i32gather_epi32((int*)Table[3], temp[3], 4);

        X[i + 4] = _mm_xor_si128(_mm_xor_si128(_mm_xor_si128(_mm_xor_si128(Y[0], Y[1]), Y[2]), Y[3]), X[i]);
    }

    // Extract the highest 32-bit integers
    Y[0] = _mm_setr_epi32(_mm_cvtsi128_si32(_mm_srli_si128(X[35], 0)), _mm_cvtsi128_si32(_mm_srli_si128(X[34], 0)), _mm_cvtsi128_si32(_mm_srli_si128(X[33], 0)), _mm_cvtsi128_si32(_mm_srli_si128(X[32], 0)));

    // Extract the second highest 32-bit integers
    Y[1] = _mm_setr_epi32(_mm_cvtsi128_si32(_mm_srli_si128(X[35], 4)), _mm_cvtsi128_si32(_mm_srli_si128(X[34], 4)), _mm_cvtsi128_si32(_mm_srli_si128(X[33], 4)), _mm_cvtsi128_si32(_mm_srli_si128(X[32], 4)));

    // Extract the second lowest 32-bit integers
    Y[2] = _mm_setr_epi32(_mm_cvtsi128_si32(_mm_srli_si128(X[35], 8)), _mm_cvtsi128_si32(_mm_srli_si128(X[34], 8)), _mm_cvtsi128_si32(_mm_srli_si128(X[33], 8)), _mm_cvtsi128_si32(_mm_srli_si128(X[32], 8)));

    // Extract the lowest 32-bit integers
    Y[3] = _mm_setr_epi32(_mm_cvtsi128_si32(_mm_srli_si128(X[35], 12)), _mm_cvtsi128_si32(_mm_srli_si128(X[34], 12)), _mm_cvtsi128_si32(_mm_srli_si128(X[33], 12)), _mm_cvtsi128_si32(_mm_srli_si128(X[32], 12)));

    for (int i = 0; i < 4; ++i) {
        _mm_storeu_si128((__m128i*)&ciphertext[i * 4], Y[i]);
    }
}

// Decryption function
void SM4_Decrypt(uint32_t ciphertext[16], __m128i rk[32], uint32_t decryptedtext[16]) {
    __m128i X[36], temp[4], Y[4];
    for (int i = 0; i < 4; ++i) {
        __m128i vindex = _mm_setr_epi32(i, i + 4, i + 8, i + 12);
        X[i] = _mm_i32gather_epi32((int*)ciphertext, vindex, 4);
    }

    __m128i mask = _mm_setr_epi32(0xff, 0xff, 0xff, 0xff);
    for (int i = 0; i < 32; i++) {
        X[i + 4] = _mm_xor_si128(_mm_xor_si128(_mm_xor_si128(X[i + 1], X[i + 2]), X[i + 3]), rk[31 - i]);

        temp[0] = _mm_and_si128(_mm_srli_epi32(X[i + 4], 24), mask);
        Y[0] = _mm_i32gather_epi32((int*)Table[0], temp[0], 4);

        temp[1] = _mm_and_si128(_mm_srli_epi32(X[i + 4], 16), mask);
        Y[1] = _mm_i32gather_epi32((int*)Table[1], temp[1], 4);

        temp[2] = _mm_and_si128(_mm_srli_epi32(X[i + 4], 8), mask);
        Y[2] = _mm_i32gather_epi32((int*)Table[2], temp[2], 4);

        temp[3] = _mm_and_si128(X[i + 4], mask);
        Y[3] = _mm_i32gather_epi32((int*)Table[3], temp[3], 4);

        X[i + 4] = _mm_xor_si128(_mm_xor_si128(_mm_xor_si128(_mm_xor_si128(Y[0], Y[1]), Y[2]), Y[3]), X[i]);
    }

    // Extract the highest 32-bit integers
    Y[0] = _mm_setr_epi32(_mm_cvtsi128_si32(_mm_srli_si128(X[35], 0)), _mm_cvtsi128_si32(_mm_srli_si128(X[34], 0)), _mm_cvtsi128_si32(_mm_srli_si128(X[33], 0)), _mm_cvtsi128_si32(_mm_srli_si128(X[32], 0)));

    // Extract the second highest 32-bit integers
    Y[1] = _mm_setr_epi32(_mm_cvtsi128_si32(_mm_srli_si128(X[35], 4)), _mm_cvtsi128_si32(_mm_srli_si128(X[34], 4)), _mm_cvtsi128_si32(_mm_srli_si128(X[33], 4)), _mm_cvtsi128_si32(_mm_srli_si128(X[32], 4)));

    // Extract the second lowest 32-bit integers
    Y[2] = _mm_setr_epi32(_mm_cvtsi128_si32(_mm_srli_si128(X[35], 8)), _mm_cvtsi128_si32(_mm_srli_si128(X[34], 8)), _mm_cvtsi128_si32(_mm_srli_si128(X[33], 8)), _mm_cvtsi128_si32(_mm_srli_si128(X[32], 8)));

    // Extract the lowest 32-bit integers
    Y[3] = _mm_setr_epi32(_mm_cvtsi128_si32(_mm_srli_si128(X[35], 12)), _mm_cvtsi128_si32(_mm_srli_si128(X[34], 12)), _mm_cvtsi128_si32(_mm_srli_si128(X[33], 12)), _mm_cvtsi128_si32(_mm_srli_si128(X[32], 12)));

    for (int i = 0; i < 4; ++i) {
        _mm_storeu_si128((__m128i*)&decryptedtext[i * 4], Y[i]);
    }
}

// Init table
void init_table() {
    for (int i = 0; i < 256; i++) {
        uint32_t val = Sbox[i];
        Table[0][i] = L(val << 24);
        Table[1][i] = L(val << 16);
        Table[2][i] = L(val << 8);
        Table[3][i] = L(val);
    }
}

// Utility function to print 128-bit blocks
void print_block(const uint32_t block[16]) {
    for (int i = 0; i < 16; i++) {
        printf("%08x ", block[i]);
    }
    printf("\n");
}
