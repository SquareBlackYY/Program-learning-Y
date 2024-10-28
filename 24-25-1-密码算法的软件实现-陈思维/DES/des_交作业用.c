#include <stdio.h>
#include <string.h>
#include <time.h>

// PC-1表，用于密钥置换
const static int PC1_TABLE[56] = {
    57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4};

// 左移位数表
const static int LOOP_TABLE[16] = {
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

// PC-2表，用于生成子密钥
const static int PC2_TABLE[48] = {
    14, 17, 11, 24, 1, 5,
    3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32};

// 置换IP表
const static int IP_TABLE[64] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7};

// 扩展运算表
const static int EXPANSION_TABLE[48] = {
    32, 1, 2, 3, 4, 5,
    4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32, 1};

// S盒
const static int S_BOX[8][4][16] = {
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
const static int P_TABLE[32] = {
    16, 7, 20, 21,
    29, 12, 28, 17,
    1, 15, 23, 26,
    5, 18, 31, 10,
    2, 8, 24, 14,
    32, 27, 3, 9,
    19, 13, 30, 6,
    22, 11, 4, 25};

/**
 * @brief DES 密钥生成
 * @param key 种子密钥
 * @param rk 生成的加密轮密钥
 */
void des_key_schedule(int key[], int rk[][48]);

/**
 * @brief DES 加密
 * @param pt 明文
 * @param rk 加密轮密钥
 * @param ct 加密得到的密文
 */
void des_encrypt(int pt[], int rk[][48], int ct[]);

/**
 * @brief DES 解密
 * @param ct 密文
 * @param rkd 解密轮密钥
 * @param pt 解密得到的明文
 */
void des_decrypt(int ct[], int rkd[][48], int pt[]);

int main()
{
    // 明文
    int pt[64], pt2[64];
    // 密文
    int ct[64];
    // 种子密钥
    int key[64];
    // 加密轮密钥
    int rk[16][48];
    // 解密轮密钥
    int rkd[16][48];

    const char *plain_text = "1000000001000000001000000001000000001000000001000000001000000001";
    const char *seed_key = "1000000000100000000100000100100000000000000000100000000001000000";
    for (int i = 0; i < 64; ++i)
    {
        pt[i] = plain_text[i] == '1' ? 1 : 0;
        key[i] = seed_key[i] == '1' ? 1 : 0;
    }

    // 密钥生成
    des_key_schedule(key, rk);

    clock_t start = clock();
    
    for (int i = 0; i < (1 << 20); ++i)
    {
        // 加密
        des_encrypt(pt, rk, ct);
    }

    clock_t end = clock();

    printf("Speed : %lf Mbps\n", 64 / ((double)(end - start) / CLOCKS_PER_SEC));

    for (int i = 0; i < 16; ++i)
        memcpy(rkd[i], rk[15 - i], 48 * sizeof(int));

    // 解密
    des_decrypt(ct, rkd, pt2);
    printf("解密结果：\t");
    for (int i = 0; i < 64; ++i)
        printf("%d", pt2[i]);
    printf("\n");
    
    printf("明文：\t\t");
    printf("%s\n", plain_text);
    
    return 0;
}

void des_key_schedule(int key[], int rk[][48])
{
    int temp[56], *C = temp, *D = temp + 28;

    // 置换选择 1
    for (int i = 0; i < 56; ++i)
        temp[i] = key[PC1_TABLE[i] - 1];
    
    for (int i = 0; i < 16; ++i)
    {
        // 循环左移
        {
            int n = LOOP_TABLE[i], temp[2];

            memcpy(temp, C, n * sizeof(int));
            memmove(C, C + n, (28 - n) * sizeof(int));
            memcpy(C + 28 - n, temp, n * sizeof(int));

            memcpy(temp, D, n * sizeof(int));
            memmove(D, D + n, (28 - n) * sizeof(int));
            memcpy(D + 28 - n, temp, n * sizeof(int));
        }

        // 置换选择 2
        for (int j = 0; j < 48; ++j)
            rk[i][j] = temp[PC2_TABLE[j] - 1];
    }
}

void des_encrypt(int pt[], int rk[][48], int ct[])
{
    int temp[64], *L = temp, *R = temp + 32, right[32];

    // 初始置换 IP
    for (int i = 0; i < 64; ++i)
        temp[i] = pt[IP_TABLE[i] - 1];

    // 轮函数
    for (int i = 0; i < 16; ++i)
    {
        int *round_key = rk[i];
        memcpy(right, R, 32 * sizeof(int));

        // F 函数
        {
            // 选择扩展运算 E
            int temp[48];
            for(int i = 0; i < 48; ++i)
                temp[i] = R[EXPANSION_TABLE[i] - 1];

            // 子密钥异或
            for(int i = 0; i < 48; ++i)
                temp[i] ^= round_key[i];

            // 选择压缩运算 S
            for (int i = 0, *p = temp, *r = R; i < 8; ++i, p += 6, r += 4)
            {
                int num = S_BOX[i][(p[0] << 1) + p[5]][(p[1] << 3) + (p[2] << 2) + (p[3] << 1) + p[4]];
                for (int i = 0; i < 4; ++i)
                    r[i] = num >> (3 - i) & 1;
            }
            
            // 置换运算 P
            memcpy(temp, R, 32 * sizeof(int));
            for (int i = 0; i < 32; ++i)
                R[i] = temp[P_TABLE[i] - 1];
        }

        // 异或
        for (int i = 0; i < 32; ++i)
            L[i] ^= R[i];

        // 交换
        if (i != 15)
        {
            memcpy(R, L, 32 * sizeof(int));
            memcpy(L, right, 32 * sizeof(int));
        }
        else{
            memcpy(R, right, 32 * sizeof(int));
        }
    }

    // 初始逆置换 IP^-1
    for (int i = 0; i < 64; ++i)
        ct[IP_TABLE[i] - 1] = temp[i];
}

void des_decrypt(int pt[], int rkd[][48], int ct[])
{
    int temp[64], *L = temp, *R = temp + 32, right[32];

    // 初始置换 IP
    for (int i = 0; i < 64; ++i)
        temp[i] = pt[IP_TABLE[i] - 1];

    // 轮函数
    for (int i = 0; i < 16; ++i)
    {
        int *round_key = rkd[i];
        memcpy(right, R, 32 * sizeof(int));

        // F 函数
        {
            // 选择扩展运算 E
            int temp[48];
            for(int i = 0; i < 48; ++i)
                temp[i] = R[EXPANSION_TABLE[i] - 1];

            // 子密钥异或
            for(int i = 0; i < 48; ++i)
                temp[i] ^= round_key[i];

            // 选择压缩运算 S
            for (int i = 0, *p = temp, *r = R; i < 8; ++i, p += 6, r += 4)
            {
                int num = S_BOX[i][(p[0] << 1) + p[5]][(p[1] << 3) + (p[2] << 2) + (p[3] << 1) + p[4]];
                for (int i = 0; i < 4; ++i)
                    r[i] = num >> (3 - i) & 1;
            }
            
            // 置换运算 P
            memcpy(temp, R, 32 * sizeof(int));
            for (int i = 0; i < 32; ++i)
                R[i] = temp[P_TABLE[i] - 1];
        }

        // 异或
        for (int i = 0; i < 32; ++i)
            L[i] ^= R[i];

        // 交换
        if (i != 15)
        {
            memcpy(R, L, 32 * sizeof(int));
            memcpy(L, right, 32 * sizeof(int));
        }
        else{
            memcpy(R, right, 32 * sizeof(int));
        }
    }

    // 初始逆置换 IP^-1
    for (int i = 0; i < 64; ++i)
        ct[IP_TABLE[i] - 1] = temp[i];
}
