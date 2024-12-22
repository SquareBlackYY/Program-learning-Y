// 128/128
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
        RC[i] = (s[0]<<5)|(s[1]<<4)|(s[2]<<3)|(s[3]<<2)|(s[4]<<1)|s[5];
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
    for (int i = 0; i < 3; i++) x[4*i] ^= L[i];
}

void TKeySchedule(uint8_t key[16], uint8_t rtk[40][8]) {
    uint8_t TK[16], tmp[16];
    for (int i = 0; i < 16; i++) TK[i] = key[i];
    for (int i = 0; i < 8; i++) rtk[0][i] = TK[i];
    
    int Pt[16] = { 9,15,8,13,10,14,12,11,0,1,2,3,4,5,6,7 };
    for (int r = 1; r < 40; r++) {
        for (int i = 0; i < 16; i++) tmp[i] = TK[Pt[i]]; // 置换
        for (int i = 0; i < 16; i++) TK[i] = tmp[i]; // 赋值
        for (int i = 0; i < 8; i++) rtk[r][i] = TK[i]; // 取值    
    }
}

// 打印rtk
void PrintRtk(uint8_t rtk[40][8]) {
    for (int i = 0; i < 40; i++) {
        printf("[%02d] ", i+1);
        for (int j = 0; j < 8; j++) {
            printf("%02x ", rtk[i][j]);
        }
        printf("\n");
    }
}

// AddRoundTweakey
void AddRoundTweakey(uint8_t x[16], uint8_t rtk[8]) {
    for (int i = 0; i < 8; i++) x[i] = x[i]^rtk[i];
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
    uint8_t m[16] = {1,0,1,1,1,0,0,0,0,1,1,0,1,0,1,0};
    uint8_t res[16] = {0};
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

void Encryption(uint8_t pt[16], uint8_t rtk[40][8], uint8_t rc[63], uint8_t ct[16]) {
    uint8_t x[16];
    for (int i = 0; i < 16; i++) x[i] = pt[i]; // 初始化

    for (int round = 0; round < 40; round++) {
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
	uint8_t rtk[40][8];
	uint8_t key[16] = { 0x4f, 0x55, 0xcf, 0xb0, 0x52, 0x0c, 0xac, 0x52, 
						0xfd, 0x92, 0xc1, 0x5f, 0x37, 0x07, 0x3e, 0x93 };
	uint8_t pt[16] = { 0xf2, 0x0a, 0xdb, 0x0e, 0xb0, 0x8b, 0x64, 0x8a, 
					   0x3b, 0x2e, 0xee, 0xd1, 0xf0, 0xad, 0xda, 0x14 };
	uint8_t ct[16] = { 0 };
	uint8_t rc[63];
	GetRC(rc);
	TKeySchedule(key, rtk);
	//PrintRtk(rtk);
	Encryption(pt, rtk, rc, ct);
    for(int i=0; i<16; i++)
		printf("%02x", ct[i]);
}
