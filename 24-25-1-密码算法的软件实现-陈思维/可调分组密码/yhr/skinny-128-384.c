// 128/384
// 函数定义的主体
#include <stdio.h>
#include <stdint.h>

// ‘|’按位或， ‘||’逻辑或  ---字节替换，不需要SBOX
int SubBytes(uint8_t n) {
    int x[8] = { 0 }; // 初始化 x 数组
    for (int i = 0; i < 8; i++) x[i] = (n >> (7 - i)) & 1;
    for (int i = 0; i < 4; i++) {
        x[3] ^= !(x[0] || x[1]);
        x[7] ^= !(x[4] || x[5]);
        //if (i != 3) PS(x);
        if (i != 3) {
            int p[8] = { 5, 6, 0, 1, 3, 7, 4, 2 };
            int y[8];
            for (int i = 0; i < 8; i++) y[i] = x[p[i]];
            for (int i = 0; i < 8; i++) x[i] = y[i];
        }
        else {
            // 直接交换而不是使用函数
            int temp = x[5]; x[5] = x[6]; x[6] = temp;
        }
    }
    int res = 0;
    for (int i = 0; i < 8; i++) res += x[i] << (7 - i);
    return res;
}

// 获取轮常量
void GetRC(uint8_t RC[63]) {
    int s[6] = { 0 };
    for (int i = 0; i < 63; i++) {
        int t = s[0] ^ s[1] ^ 1;
        for (int i = 1; i < 6; i++)
            s[i - 1] = s[i];
        s[5] = t;
        RC[i] = (s[0] << 5) | (s[1] << 4) | (s[2] << 3) | (s[3] << 2) | (s[4] << 1) | s[5];
    }
}

// 字节替代
void SubCell(uint8_t x[16]) {
    for (int i = 0; i < 16; i++)
        x[i] = SubBytes(x[i]);
}

// 异或轮常量
void AddConstants(uint8_t x[16], uint8_t c) {
    int L[3] = { c & 0xf ,c >> 4 , 0x2 };
    for (int i = 0; i < 3; i++) x[4 * i] ^= L[i];
}

int LFSR_TK2(uint8_t s) {
    uint8_t x[8], res=0;
    for (int i = 0; i < 8; i++)
        x[i] = (s >> (7 - i)) & 1;
    uint8_t t = x[0] ^ x[2];
    for (int i = 0; i < 7; i++)
        x[i] = x[i + 1];
    x[7] = t;
    for (int i = 0; i < 8; i++)
        res += x[i] << (7 - i);
    return res;
}

int LFSR_TK3(uint8_t s) {
    uint8_t x[8], res=0;
    res = (s&1) ^ ((s>>6)&1);
    return (res<<7) ^ (s>>1);
}

void TKeySchedule(uint8_t key[48], uint8_t rtk[56][8]) {
    uint8_t TK1[16], TK2[16], TK3[16], tmp[16];
    for (int i = 0; i < 16; i++) {
        TK1[i] = key[i];
        TK2[i] = key[i + 16];
        TK3[i] = key[i + 32];
    }
    for (int i = 0; i < 8; i++) rtk[0][i] = TK1[i] ^ TK2[i] ^ TK3[i];

    int Pt[16] = { 9,15,8,13,10,14,12,11,0,1,2,3,4,5,6,7 };
    for (int r = 1; r < 56; r++) {
        // 更新TK1
        for (int i = 0; i < 16; i++) tmp[i] = TK1[Pt[i]]; // 置换
        for (int i = 0; i < 16; i++) TK1[i] = tmp[i]; // 赋值
        
        // 更新TK2
        for (int i = 0; i < 16; i++) tmp[i] = TK2[Pt[i]]; // 置换
        for (int i = 0; i < 16; i++) TK2[i] = tmp[i]; // 赋值
        for (int i = 0; i < 8; i++) TK2[i] = LFSR_TK2(TK2[i]); // LFSR
        
        // 更新TK3
        for (int i = 0; i < 16; i++) tmp[i] = TK3[Pt[i]]; // 置换
        for (int i = 0; i < 16; i++) TK3[i] = tmp[i]; // 赋值
        for (int i = 0; i < 8; i++) TK3[i] = LFSR_TK3(TK3[i]); // LFSR

        // 赋值
        for (int i = 0; i < 8; i++) rtk[r][i] = TK1[i] ^ TK2[i] ^ TK3[i]; // 取值    
    }
}

// 打印rtk
void PrintRtk(uint8_t rtk[56][8]) {
    for (int i = 0; i < 56; i++) {
        printf("[%02d] ", i + 1);
        for (int j = 0; j < 8; j++) {
            printf("%02x ", rtk[i][j]);
        }
        printf("\n");
    }
}

// AddRoundTweakey
void AddRoundTweakey(uint8_t x[16], uint8_t rtk[8]) {
    for (int i = 0; i < 8; i++) x[i] = x[i] ^ rtk[i];
}

// 行移位
void ShiftRows(uint8_t x[16]) {
    int p[16] = { 0,1,2,3,7,4,5,6,10,11,8,9,13,14,15,12 };
    uint8_t tmp[16];
    for (int i = 0; i < 16; i++) tmp[i] = x[p[i]];
    for (int i = 0; i < 16; i++) x[i] = tmp[i];
}

// 列混淆
void MixColumns(uint8_t x[16]) {
    uint8_t m[16] = { 1,0,1,1,1,0,0,0,0,1,1,0,1,0,1,0 };
    uint8_t res[16] = { 0 };
    // 进行矩阵乘法
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                res[i * 4 + j] ^= m[i * 4 + k] * x[k * 4 + j];
            }
        }
    }
    for (int i = 0; i < 16; i++) x[i] = res[i];
}

void Print16byte(uint8_t x[16]) {
    for (int i = 0; i < 16; i++) {
        printf("%02x ", x[i]);
        if ((i + 1) % 4 == 0)
            printf("\n");
    }
    printf("\n");
}

void Encryption(uint8_t pt[16], uint8_t rtk[56][8], uint8_t rc[63], uint8_t ct[16]) {
    uint8_t x[16];
    for (int i = 0; i < 16; i++) x[i] = pt[i]; // 初始化

    for (int round = 0; round < 56; round++) {
        SubCell(x);
        //printf("S C(round[%02d]):\n", round); Print16byte(x);

        AddConstants(x, rc[round]);
        //printf("AC(round[%02d]):\n", round); Print16byte(x);

        AddRoundTweakey(x, rtk[round]);
        //printf("ART(round[%02d]):\n", round); Print16byte(x);

        ShiftRows(x);
        //printf("S R(round[%02d]):\n", round); Print16byte(x);

        MixColumns(x);
        //printf("M C(round[%02d]):\n", round); Print16byte(x);
    }

    for (int i = 0; i < 16; i++) ct[i] = x[i];
}

int main() {
    uint8_t rtk[56][8];
    uint8_t key[48] = { 0xdf, 0x88, 0x95, 0x48, 0xcf, 0xc7, 0xea, 0x52, 0xd2, 0x96, 0x33, 0x93, 0x01, 0x79, 0x74, 0x49 ,
                        0xab, 0x58, 0x8a, 0x34, 0xa4, 0x7f, 0x1a, 0xb2, 0xdf, 0xe9, 0xc8, 0x29, 0x3f, 0xbe, 0xa9, 0xa5 ,
                        0xab, 0x1a, 0xfa, 0xc2, 0x61, 0x10, 0x12, 0xcd, 0x8c, 0xef, 0x95, 0x26, 0x18, 0xc3, 0xeb, 0xe8 };
    uint8_t pt[16] = { 0xa3, 0x99, 0x4b, 0x66, 0xad, 0x85, 0xa3, 0x45, 0x9f, 0x44, 0xe9, 0x2b, 0x08, 0xf5, 0x50, 0xcb };
    uint8_t ct[16] = { 0 };
    uint8_t rc[63];
    GetRC(rc);
    TKeySchedule(key, rtk);
    //PrintRtk(rtk);
    Encryption(pt, rtk, rc, ct);
    for(int i=0; i<16; i++) printf("%02x", ct[i]);
    //printf("%02X", LFSR_TK3(0X3C));
}