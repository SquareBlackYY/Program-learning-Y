#include <iostream>
using namespace std;

uint8_t sbox[16] = {0x6, 0x4, 0xc, 0x5, 0x0, 0x7, 0x2, 0xe, 0x1, 0xf, 0x3, 0xd, 0x8, 0xa, 0x9, 0xb};

uint16_t round_f(uint16_t);

int main()
{
    uint16_t c = round_f(0xffff);
    printf("%x\n", c);
    return 0;
}

uint16_t round_f(uint16_t m)
{
    uint16_t a = sbox[m >> 12], b = sbox[(m >> 8) & 0xf], c = sbox[(m >> 4) & 0xf], d = sbox[m & 0xf];
    uint16_t cipher = a >> 3 << 15;
    return cipher;
}