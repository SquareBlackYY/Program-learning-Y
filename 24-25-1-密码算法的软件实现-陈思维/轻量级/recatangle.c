#include <stdio.h>
#include <stdint.h>
#include <string.h>
uint8_t RC[25] = {0x01, 0x02, 0x04, 0x09, 0x12, 0x05, 0x0b, 0x16, 0x0c, 0x19, 0x13, 0x07, 0x0f, 0x1f, 0x1e, 0x1c, 0x18, 0x11, 0x03, 0x06, 0x0d, 0x1b, 0x17, 0x0e, 0x1d};

uint64_t sTOs_table[8][256], p_table[8][256], inv_p_table[8][256];
int p_permute[64], inv_p_permute[64];

//ANF型S盒
void s_box(uint16_t plaintext[4],int n)
{
    uint16_t tmp[4];
    memcpy(tmp, plaintext, sizeof(tmp));

    plaintext[0] = tmp[0] ^ (tmp[0] & tmp[1]) ^ tmp[2] ^ tmp[3];
    plaintext[1] = ((1 << n) - 1) ^ tmp[0] ^ tmp[1] ^ tmp[2] ^ (tmp[1] & tmp[3]);
    plaintext[2] = ((1 << n) - 1) ^ (tmp[0] & tmp[1]) ^ tmp[2] ^ (tmp[0] & tmp[2]) ^ (tmp[1] & tmp[2]) ^ (tmp[0] & tmp[1] & tmp[2]) ^ tmp[3] ^ (tmp[2] & tmp[3]);
    plaintext[3] = tmp[1] ^ (tmp[0] & tmp[2]) ^ (tmp[1] & tmp[2]) ^ tmp[3] ^ (tmp[0] & tmp[3]) ^ (tmp[1] & tmp[2] & tmp[3]);
}

//密钥扩展算法
void KeyExpansion(uint16_t round_key[26][4], uint64_t round_key_64[26], uint8_t seed_key[10])
{

    uint16_t table[5];
    for (int i = 0; i < 5; i++)
        table[4 - i] = ((uint16_t)seed_key[2 * i + 1] << 8) ^ seed_key[2 * i];
    for (int i = 0; i < 4; i++)
        round_key[0][i] = table[i];
    for (int r = 1; r < 26; r++)
    {
        uint16_t tmp[4], tmp_table[5];
        for (int i = 0; i < 4; i++)
            tmp[i] = table[i] & 0xf;
        s_box(tmp, 4);
        for (int i = 0; i < 4; i++)
            table[i] = (table[i] & 0xfff0) ^ tmp[i];

        for (int i = 0; i < 5; i++)
            tmp_table[i] = table[i];
        table[0] = (uint16_t)(tmp_table[0] << 8 | tmp_table[0] >> 8) ^ tmp_table[1] ^ RC[r - 1];
        table[1] = tmp_table[2];
        table[2] = tmp_table[3];
        table[3] = (uint16_t)(tmp_table[3] << 12 | tmp_table[3] >> 4) ^ tmp_table[4];
        table[4] = tmp_table[0];
        for (int i = 0; i < 4; i++)
            round_key[r][i] = table[i];
    }

    for (int i = 0; i < 26; i++)
        round_key_64[i] = ((uint64_t)round_key[i][3] << 48) | ((uint64_t)round_key[i][2] << 32) | ((uint64_t)round_key[i][1] << 16) | round_key[i][0];
}

//基于比特切片的加密
void encrypt_bitSlice(uint16_t plaintext[4], uint16_t round_key[26][4], uint16_t ciphertext[4]){
    uint16_t table[4];
    for(int i =0;i<4;i++)
        table[3 - i] = plaintext[i];
    for(int r =0;r<25;r++){
        for(int i=0;i<4;i++)
            table[i] ^= round_key[r][i];
        s_box(table,16);
        table[1] = table[1] << 1 | table[1] >> 15;
        table[2] = table[2] << 12 | table[2] >> 4;
        table[3] = table[3] << 13 | table[3] >> 3;
    }
    for(int i=0;i<4;i++)
        table[i] ^= round_key[25][i];
    for(int i =0;i<4;i++)
        ciphertext[i] = table[i];
}

//生成拉线部分对应位置的数组
void init_p_permute(){
    int num = 0;
    for(int i =0;i<16;i++)
        for(int j =0;j<4;j++){
            p_permute[cnt] = (i + 16 * j) % 64;
            num++;
        }
}
void init_inv_p_permute(){
    num = 0;
    for(int i =0;i<4;i++)
        for(int j =0;j<16;j++){
            inv_p_permute[cnt] = (i + 4 * j) % 64;
            num++;
        }
}

//生成线性层p以及inv_p的表
void init_p_table(){
    uint64_t tmp;
    int tmp_bit[64], p_bit[64];
    for(int i=0;i<8;i++)
        for(int j=0;j<256;j++){
            uint64_t p =0;
            tmp = (uint64_t)(j << (8 * i));
            for(int k =0;k<64;k++){
                tmp_bit[k] = tmp & 1;
            }
            for(int k=0;k<64;k++)
                p_bit[k] = tmp_bit[p_permute[k]];
            for(int k =0;k<64;k++)
                p += p_bit[k] << k;
            p_table[i][j] = p;
        }
}
void init_inv_p_table(){
    uint64_t tmp;
    int tmp_bit[64], p_bit[64];
    for(int i=0;i<8;i++)
        for(int j=0;j<256;j++){
            uint64_t p =0;
            tmp = (uint64_t)(j << (8 * i));
            for(int k =0;k<64;k++){
                tmp_bit[k] = tmp & 1;
            }
            for(int k=0;k<64;k++)
                p_bit[k] = tmp_bit[inv_p_permute[k]];
            for(int k =0;k<64;k++)
                p += p_bit[k] << k;
            inv_p_table[i][j] = p;
        }
}



int main()
{
    uint16_t plaintext[4] = {0x0000, 0x0000, 0x0000, 0x0000};
    uint16_t ciphertext[4];
    uint8_t seed_key[10] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint64_t round_key_64[26];
    uint16_t round_key[26][4];
    printf("初始明文为：");
    for(int i= 0;i<4;i++)
        printf("%04x ",plaintext[i]);
    KeyExpansion(round_key, round_key_64, seed_key);
    encrypt_bitSlice(plaintext, round_key, ciphertext);
    printf("\n加密得到的密文为：");
    for(int i= 0;i<4;i++)
        printf("%04x ",ciphertext[i]);
    return 0;
}