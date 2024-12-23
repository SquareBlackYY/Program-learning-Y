#include <stdint.h>
#include <stdio.h>
#include <arm_neon.h>
#include <memory.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#define TEST_BLOCK_SIZE (1ULL << 25) // 测试分组（16字节）
#define TEST_ROUND 10 // 测试轮数

// S盒 用于加密时字节代换
static const uint8_t S_BOX[256] = {
    // 0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, // 0
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, // 1
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, // 2
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, // 3
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, // 4
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, // 5
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, // 6
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, // 7
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, // 8
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, // 9
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, // A
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, // B
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, // C
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, // D
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, // E
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16  // F
};

// 轮常量 用于密钥扩展
static const uint8_t R_CON[11] = {
    0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};

// AES-128加密函数
void aes128_encrypt_neon(const uint8_t *p, uint8_t *c, const uint8x16_t *rk)
{
    // 加载输入块
    uint8x16_t state = vld1q_u8(p);

    for (int i = 0; i < 9; ++i)
        state = vaesmcq_u8(vaeseq_u8(state, rk[i]));

    state = vaeseq_u8(state, rk[9]);
    state = veorq_u8(state, rk[10]);
    vst1q_u8(c, state);
}

// AES-128解密函数
void aes128_decrypt_neon(const uint8_t *c, uint8_t *p, const uint8x16_t *rk)
{
    // 加载输入块
    uint8x16_t state = vld1q_u8(c);

    
    for (int i = 10; i > 1; i--)
       state = vaesimcq_u8(vaesdq_u8(state, vaesimcq_u8(rk[i])));

    state = veorq_u8(state, vaesimcq_u8(rk[0]));
    vst1q_u8(p, state);
}

// AES-128轮密钥初始化
void init_round_keys(uint8x16_t *rk, const uint8_t *k)
{
    uint8_t roundKeys[11][16];

    for (int i = 0; i < 16; ++i)
        roundKeys[0][i] = k[i];

    int rcon_index = 1;

    for (int i = 1; i < 11; ++i)
    {
        roundKeys[i][0] = S_BOX[roundKeys[i - 1][13]] ^ R_CON[rcon_index++] ^ roundKeys[i - 1][0];
        roundKeys[i][1] = S_BOX[roundKeys[i - 1][14]] ^ roundKeys[i - 1][1];
        roundKeys[i][2] = S_BOX[roundKeys[i - 1][15]] ^ roundKeys[i - 1][2];
        roundKeys[i][3] = S_BOX[roundKeys[i - 1][12]] ^ roundKeys[i - 1][3];

        for (int j = 4; j < 16; ++j)
            roundKeys[i][j] = roundKeys[i][j - 4] ^ roundKeys[i - 1][j];
    }

    for (int i = 0; i < 11; ++i)
        rk[i] = vld1q_u8(roundKeys[i]);
}

int main()
{
    const uint8_t key[16] = {
        0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff};
    const uint8_t pt[16] = {
        0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff};
    uint8_t ct[16];
    uint8_t de_pt[16];

    // 打印输入明文
    printf("输入明文: ");
    for (int i = 0; i < 16; ++i)
        printf("%02x ", pt[i]);
    printf("\n");

    uint8x16_t rk[11];
    init_round_keys(rk, key);

    // 执行AES-128加密
    aes128_encrypt_neon(pt, ct, rk);

    // 打印加密结果
    printf("加密结果: ");
    for (int i = 0; i < 16; ++i)
        printf("%02x ", ct[i]);
    printf("\n");

    // 执行AES-128解密
    aes128_decrypt_neon(ct, de_pt, rk);

    // 打印解密结果
    printf("解密结果: ");
    for (int i = 0; i < 16; ++i)
        printf("%02x ", de_pt[i]);
    printf("\n");

    uint8_t *pts = (uint8_t *)malloc(TEST_BLOCK_SIZE * 16 * sizeof(uint8_t));
    uint8_t *cts = (uint8_t *)malloc(TEST_BLOCK_SIZE * 16 * sizeof(uint8_t));
    uint8_t *de_pts = (uint8_t *)malloc(TEST_BLOCK_SIZE * 16 * sizeof(uint8_t));

    // 加密速度测试
    for (int j = 0; j < TEST_ROUND; ++j)
    {
        // 随机生成数据
        for (size_t i = 0; i < TEST_BLOCK_SIZE * 16; ++i)
            pts[i] = (uint8_t)(rand() % 256);

        // 加密
        uint8_t *pts_ptr = pts, *cts_ptr = cts;
        clock_t start = clock();
        for (int i = 0; i < TEST_BLOCK_SIZE; ++i)
        {
            aes128_encrypt_neon(pts_ptr, cts_ptr, rk);
            pts_ptr += 16;
            cts_ptr += 16;
        }
        clock_t end = clock();
        unsigned long long data = 16 * TEST_BLOCK_SIZE / 1024 / 1024; // 测试数据大小（MB）
        printf("第%2d轮, 加密了 %llu MB 数据, 速度: %.2f Mbps\n",j + 1 ,data, 8 * data / ((double)(end - start) / CLOCKS_PER_SEC));

        // 解密
        cts_ptr = cts;
        uint8_t *de_pt_ptr = de_pts;
        for (int i = 0; i < TEST_BLOCK_SIZE; ++i)
        {
            aes128_decrypt_neon(cts_ptr, de_pt_ptr, rk);
            cts_ptr += 16;
            de_pt_ptr += 16;
        }

        // 解密正确性验证
        bool isSame = true;
        for (int i = 0; i < TEST_BLOCK_SIZE * 16; ++i)
        {
            if (pts[i] != de_pts[i])
            {
                isSame = false;
                break;
            }
        }
        printf("%s", isSame ? "验证通过\n" : "验证失败\n");

        //if ()
    }

    return 0;
}