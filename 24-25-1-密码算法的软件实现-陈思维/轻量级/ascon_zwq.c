#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h> 

#define ROR(x,a) (uint64_t)((x>>a)|(x<<(64-a)))

const int r = 64;
uint64_t Nonce[2] = { 0x0001020304050607, 0x08090a0b0c0d0e0f };
uint64_t key[2] = { 0x0001020304050607,0x08090A0B0C0D0E0F };
uint8_t a[9] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
uint64_t cr[12] = { 0x00000000000000f0,0x00000000000000e1,0x00000000000000d2,0x00000000000000c3,0x00000000000000b4,0x00000000000000a5,0x0000000000000096,0x0000000000000087,0x0000000000000078,0x0000000000000069,0x000000000000005a,0x000000000000004b };
uint64_t IV = 0x80400c0600000000;

void ascon_encrypt(uint64_t P[3], uint64_t key[16], uint64_t C[2], uint64_t T[2], uint64_t A[3]);
void ascon_decrypt(uint64_t C[3], uint64_t key[2], uint64_t PT[3], uint64_t T[2], uint64_t A[2]);
void P(uint64_t tmp[5], int a);
void pad_to_64_bits(uint8_t* input, size_t input_length, uint8_t** padded_output, size_t* output_length);

int main() {
    uint8_t pt[23] = { 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x16};
    uint64_t ct[3], T[2];
    uint64_t A[2] = { 0 };
    uint64_t PT[3] = { 0 };
    uint64_t DT[3] = { 0 };
    size_t input_length = sizeof(a) / sizeof(a[0]);
    uint8_t* padded_output = NULL;
    size_t output_length = 0;
    pad_to_64_bits(a, input_length, &padded_output, &output_length);
    for (size_t i = 0; i < output_length; ++i) 
        A[i / 8] |= ((uint64_t)padded_output[i]) << ((7 - (i % 8)) * 8);
    /*
    for (size_t i = 0; i < output_length / 8; ++i) {
        printf("%016llX\n", A[i]);
    }*/

    input_length = sizeof(pt) / sizeof(pt[0]);
    padded_output = NULL;
    output_length = 0;
    pad_to_64_bits(pt, input_length, &padded_output, &output_length);
    for (size_t i = 0; i < output_length; ++i) 
        PT[i / 8] |= ((uint64_t)padded_output[i]) << ((7 - (i % 8)) * 8);
    /*
    for (size_t i = 0; i < output_length / 8; ++i) {
        printf("%016llX\n", P[i]);
    }*/
    ascon_encrypt(PT, key, ct, T, A);
    printf("c:");
    for (int i = 0; i < 3; i++) {
        printf("%llx\n", ct[i]);
    }
    printf("T:");
    for (int i = 0; i < 2; i++) {
        printf("%llx\n", T[i]);
    }
    ascon_decrypt(ct, key, DT, T, A);
    printf("DT:");
    for (int i = 0; i < 3; i++) {
        printf("%016llx\n", DT[i]);
    }
    return 0;
}

void pad_to_64_bits(uint8_t* input, size_t input_length, uint8_t** padded_output, size_t* output_length) {
    // º∆À„–Ë“™ÃÓ≥‰µΩµƒ◊‹≥§∂»£®64 Œªµƒ’˚ ˝±∂£©
    size_t padded_length = ((input_length + sizeof(uint64_t) - 1) / sizeof(uint64_t)) * sizeof(uint64_t);

    // ∑÷≈‰ƒ⁄¥Ê“‘¥Ê¥¢ÃÓ≥‰∫Ûµƒ ‰≥ˆ
    *padded_output = (uint8_t*)malloc(padded_length);
    if (*padded_output == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    memset(*padded_output, 0, padded_length); // ≥ı ºªØŒ™ 0

    // ∏¥÷∆‘≠ º ‰»Î ˝æ›µΩ ‰≥ˆª∫≥Â«¯
    memcpy(*padded_output, input, input_length);

    // ÃÌº”±»ÃÿŒª 1£®…Ë÷√◊Ó∏ﬂ”––ßŒªŒ™ 1£©£¨»Áπ˚ø…ƒ‹µƒª∞£¨‘⁄◊Ó∫Û“ª∏ˆ ˝æ›◊÷Ω⁄∫Û√Ê
    if (input_length < padded_length) {
        (*padded_output)[input_length] = 0x80;
    }

    // ∏¸–¬ ‰≥ˆ≥§∂»
    *output_length = padded_length;
}

void P(uint64_t tmp[5], int a) {
    uint64_t t0, t1, t2, t3, t4;
    for (int i = 0; i < a; i++) {
        tmp[2] = tmp[2] ^ cr[12 - a + i];

        t0 = tmp[0];
        t1 = tmp[1];
        t2 = tmp[2];
        t3 = tmp[3];
        t4 = tmp[4];
        // ∏˘æ›π´ Ωº∆À„≤¢÷±Ω”∏¸–¬ tmp
        tmp[0] = t4 & t3 ^ t2 & t1 ^ t2 ^ t1 & t0 ^ t1 ^ t0; // y0
        tmp[1] = t4 ^ t3 & t2 ^ t1 & t3 ^ t2 & t1 ^ t2 ^ t1 ^ t0; // y1
        tmp[2] = t4 & t3 ^ t4 ^ t2 ^ t1 ^ 0xffffffffffffffff; // y2
        tmp[3] = t4 & t0 ^ t4 ^ t3 & t0 ^ t3 ^ t2 ^ t1 & t0; // y3
        tmp[4] = t4 & t1 ^ t4 ^ t3 ^ t1 ^ t0 & t1 ^ t1; // y4

        tmp[0] = tmp[0] ^ ROR(tmp[0], 19) ^ ROR(tmp[0], 28);
        tmp[1] = tmp[1] ^ ROR(tmp[1], 61) ^ ROR(tmp[1], 39);
        tmp[2] = tmp[2] ^ ROR(tmp[2], 1) ^ ROR(tmp[2], 6);
        tmp[3] = tmp[3] ^ ROR(tmp[3], 10) ^ ROR(tmp[3], 17);
        tmp[4] = tmp[4] ^ ROR(tmp[4], 7) ^ ROR(tmp[4], 41);
    }
        
}

void ascon_encrypt(uint64_t PT[3], uint64_t key[2], uint64_t ct[3], uint64_t T[2], uint64_t A[2]) {
    uint64_t tmp[5] = { 0 };
    tmp[0] = IV;
    for (int i = 0; i < 2; i++)
        tmp[i + 1] = key[i];
    for (int i = 0; i < 2; i++)
        tmp[i + 3] = Nonce[i];
    P(tmp, 12);
    //AD
    tmp[0] = tmp[0] ^ A[0];
    tmp[3] = tmp[3] ^ key[0];
    tmp[4] = tmp[4] ^ key[1];
    P(tmp, 6);
    tmp[0] = tmp[0] ^ A[1];
    P(tmp, 6);
    tmp[4] = tmp[4] ^ 1;
    //pt
    ct[0] = tmp[0] ^ PT[0];
    tmp[0] = ct[0];
    P(tmp, 6);
    ct[1] = tmp[0] ^ PT[1];
    tmp[0] = ct[1];
    P(tmp, 6);
    tmp[0] = tmp[0] ^ PT[2];
    ct[2] = tmp[0];
    //key
    tmp[1] = tmp[1] ^ key[0];
    tmp[2] = tmp[2] ^ key[1];
    P(tmp, 12);
    T[0] = tmp[3] ^ key[0];
    T[1] = tmp[4] ^ key[1];
}

void ascon_decrypt(uint64_t C[3], uint64_t key[2], uint64_t PT[3], uint64_t T[2], uint64_t A[2]) {
    uint64_t tmp[5] = { 0 };
    tmp[0] = IV;
    for (int i = 0; i < 2; i++)
        tmp[i + 1] = key[i];
    for (int i = 0; i < 2; i++)
        tmp[i + 3] = Nonce[i];
    P(tmp, 12);

    tmp[0] = tmp[0] ^ A[0];
    tmp[3] = tmp[3] ^ key[0];
    tmp[4] = tmp[4] ^ key[1];
    P(tmp, 6);
    tmp[0] = tmp[0] ^ A[1];
    P(tmp, 6);
    tmp[4] = tmp[4] ^ 1;

    PT[0] = tmp[0] ^ C[0];
    tmp[0] = C[0];
    P(tmp, 6);
    PT[1] = tmp[0] ^ C[1];
    tmp[0] = C[1];
    P(tmp, 6);
    tmp[0] = tmp[0] ^ C[2];
    PT[2] = tmp[0];

    tmp[1] = tmp[1] ^ key[0];
    tmp[2] = tmp[2] ^ key[1];
    P(tmp, 12);

    uint64_t T_dec[2];
    T_dec[0] = tmp[3] ^ key[0];
    T_dec[1] = tmp[4] ^ key[1];
    printf("T_dec:");
    for (int i = 0; i < 2; i++) {
        printf("%llx\n", T_dec[i]);
    }
    if (T_dec[0] != T[0] || T_dec[1] != T[1])
        printf(" ß∞‹\n");
    else
        printf("≥…π¶\n");
}
