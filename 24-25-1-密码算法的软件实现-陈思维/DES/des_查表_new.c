#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define BLOCK_SIZE (1 << 20) // 多轮加密分组大小

// PC-1表，用于密钥置换
static const uint8_t PC1_TABLE[56] = {
    57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4};

// 左移位数表
static const uint8_t LOOP_TABLE[16] = {
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

// PC-2表，用于生成子密钥
static const uint8_t PC2_TABLE[48] = {
    14, 17, 11, 24, 1, 5,
    3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32};

// 置换IP表
static const uint8_t IP_TABLE[64] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7};

// 扩展运算表
static const uint8_t EXPANSION_TABLE[48] = {
    32, 1, 2, 3, 4, 5,
    4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32, 1};

// S盒
static const uint8_t S_BOX[8][4][16] = {
    // S1
    {
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},
    // S2
    {
        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
        {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
        {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},
    // S3
    {
        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
        {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
        {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},
    // S4
    {
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
        {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
        {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
        {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},
    // S5
    {
        {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
        {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
        {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
        {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},
    // S6
    {
        {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
        {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
        {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
        {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},
    // S7
    {
        {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
        {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},
    // S8
    {
        {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
        {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
        {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
        {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}};

// P换位表
static const uint8_t P_TABLE[32] = {
    16, 7, 20, 21,
    29, 12, 28, 17,
    1, 15, 23, 26,
    5, 18, 31, 10,
    2, 8, 24, 14,
    32, 27, 3, 9,
    19, 13, 30, 6,
    22, 11, 4, 25};

uint64_t ET[4][256] = {0}; // E扩展加速表
uint32_t SPT[8][64] = {0}; // (S盒+P置换)加速表
int flag = 0;        // 标志加密还是解密

void des_key_schedule(const uint8_t *sk, uint64_t *rk);
void des_encrypt(uint64_t *pt, const uint64_t rk[], uint64_t *ct);
void des_decrypt(uint64_t *ct, const uint64_t rk[], uint64_t *pt);
void init_lookup_table();

int main()
{
    uint8_t seed_key[8] = {0b10000000, 0b00100000, 0b00010000, 0b01001000, 0b00000000, 0b00000010, 0b00000000, 0b01000000};
    uint64_t round_keys[16];

    uint64_t plain_text = 0b1000000001000000001000000001000000001000000001000000001000000001ULL;
    uint64_t cipher_text = 0;
    uint64_t decrypted_text = 0;

    // 密钥生成
    des_key_schedule(seed_key, round_keys);

    // 初始化加速表
    init_lookup_table();

    // 明文输出
    printf("明文:\n");
    for (int i = 63; i >= 0; i--)
        printf("%llu", (plain_text >> i) & 1);
    printf("\n");

    // 加密
    des_encrypt(&plain_text, round_keys, &cipher_text);

    // 加密结果
    printf("加密结果:\n");
    for (int i = 63; i >= 0; i--)
        printf("%llu", (cipher_text >> i) & 1);
    printf("\n");

    // 解密
    des_decrypt(&cipher_text, round_keys, &decrypted_text);

    // 解密结果
    printf("解密结果:\n");
    for (int i = 63; i >= 0; i--)
        printf("%llu", (decrypted_text >> i) & 1);
    printf("\n");

    // 多轮加密
    clock_t start = clock();
    for (int i = 0; i < BLOCK_SIZE; i++)
        des_encrypt(&plain_text, round_keys, &cipher_text);
    clock_t end = clock();

    // 计算速度
    uint64_t data = 8 * BLOCK_SIZE / 1024 / 1024;
    printf("加密 %llu MB 数据量，速度 : %f Mbps\n", data, 8 * data / ((double)(end - start) / CLOCKS_PER_SEC));

    return 0;
}

void des_key_schedule(const uint8_t *sk, uint64_t *rk)
{
    uint64_t key = 0;

    // 从主密钥生成56位密钥
    for (int i = 0; i < 56; i++)
    {
        uint8_t pos = PC1_TABLE[i] - 1;
        key <<= 1;
        key |= (sk[pos / 8] >> (7 - (pos % 8))) & 0x01; // 从主密钥中提取比特
    }

    // 将密钥分为左右两部分
    uint32_t C = key >> 28;
    uint32_t D = key & 0xFFFFFFF;

    // 生成16轮子密钥
    for (int r = 0; r < 16; r++)
    {
        // 左移C和D
        C = ((C << LOOP_TABLE[r]) | (C >> (28 - LOOP_TABLE[r]))) & 0xFFFFFFF;
        D = ((D << LOOP_TABLE[r]) | (D >> (28 - LOOP_TABLE[r]))) & 0xFFFFFFF;

        // 合并C和D为64位密钥
        uint64_t combined_key = ((uint64_t)C << 28) | D;

        // 通过PC-2表生成子密钥
        rk[r] = 0;
        for (int i = 0; i < 48; i++)
        {
            rk[r] <<= 1;
            rk[r] |= (combined_key >> (56 - PC2_TABLE[i])) & 0x01; // 取出子密钥
        }
    }
}

// DES 加密
void des_encrypt(uint64_t *pt, const uint64_t rk[], uint64_t *ct)
{
    for (int i = 0; i < 64; i++)
        if (*pt >> (64 - IP_TABLE[i]) & 0x01)
            *ct |= 1ULL << (63 - i);

    uint32_t *C = (uint32_t *)ct + 1;
    uint32_t *D = (uint32_t *)ct;
    uint32_t *next = (uint32_t *)pt + 1;
    uint32_t *t;

    for (int r = 0; r < 16; r++)
    {
        uint8_t *p = (uint8_t *)D;
        uint64_t temp = (ET[0][*p] | ET[1][*p + 1] | ET[2][*p + 2] | ET[3][*p + 3]) ^ (flag ? rk[15 - r] : rk[r]);
        p = (uint8_t *)&temp;
        *next = SPT[0][*p] | SPT[1][*p + 1] | SPT[2][*p + 2] | SPT[3][*p + 3] | SPT[4][*p + 4] | SPT[5][*p + 5] | SPT[6][*p + 6] | SPT[7][*p + 7];
        *next ^= *C;

        t = C;
        C = D;
        D = next;
        next = t;
    }
    *(uint32_t *)pt = *C;

    *ct = 0;
    for (int i = 0; i < 64; i++)
        if (*pt >> (63 - i) & 0x01)
            *ct |= 1ULL << (64 - IP_TABLE[i]);

    // 恢复标志位为加密状态
    flag = 0;
}

// DES 解密
void des_decrypt(uint64_t *ct, const uint64_t rk[], uint64_t *pt)
{
    flag = 1; // 设置解密
    des_encrypt(ct, rk, pt);
}

// 初始化加速表
void init_lookup_table()
{
    // 初始化 E 扩展运算表
    for (int i = 0; i < 256; i++)
        for (int j = 0; j < 48; j++)
        {
            int pos = EXPANSION_TABLE[j] - 1;
            int idx = pos / 8;
            if (i >> (7 + idx * 8 - pos) & 0x01)
                ET[idx][i] |= 1ULL << (47 - j);
        }

    // 初始化(S盒+P置换)表
    for (int i = 0; i < 64; i++)
    {
        int row = ((i & 0x20) >> 4) | (i & 0x01);
        int col = (i >> 1) & 0x0F;
        for (int j = 0; j < 32; j++)
        {
            int pos = P_TABLE[j] - 1;
            int idx = pos / 4;
            uint32_t sbox_val = S_BOX[idx][row][col];
            if (sbox_val >> (3 + idx * 4 - pos) & 0x01)
                SPT[idx][i] |= 1ULL << (31 - j);
        }
    }
}
