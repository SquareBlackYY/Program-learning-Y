#include<stdio.h>
#include<stdint.h>

#define cyc_left(x, left) (uint16_t)(((x << left) | (x >> (16 - left))) & 0xffff)

uint8_t ptable[64], pitable[64];
uint64_t p_table[8][256], s_table[8][256], pi_table[8][256];
uint16_t S[16] = { 0x6, 0x5, 0xc, 0xa, 0x1, 0xe, 0x7, 0x9, 0xb, 0x0, 0x3, 0xd, 0x8, 0xf, 0x4, 0x2 };
uint8_t RC[25] = { 0x01,0x02,0x04,0x09,0x12,0x05,0x0b,0x16,0x0c,0x19,0x13,0x07,0x0f,0x1f,0x1e,0x1c,0x18,0x11,0x03,0x06,0x0d,0x1b,0x17,0x0e,0x1d };

void sbox(uint16_t a[4], int n) {
	uint16_t tmp[4];
	for (int i = 0; i < 4; i++) 
		tmp[i] = a[i];
	a[0] = tmp[0] ^ (tmp[0] & tmp[1]) ^ tmp[2] ^ tmp[3];
	a[1] = ((1 << n) - 1) ^ tmp[0] ^ tmp[1] ^ tmp[2] ^ (tmp[1] & tmp[3]);
	a[2] = ((1 << n) - 1) ^ (tmp[0] & tmp[1]) ^ tmp[2] ^ (tmp[0] & tmp[2]) ^ (tmp[1] & tmp[2]) ^ (tmp[0] & tmp[1] & tmp[2]) ^ tmp[3] ^ (tmp[2] & tmp[3]);
	a[3] = tmp[1] ^ (tmp[0] & tmp[2]) ^ (tmp[1] & tmp[2]) ^ tmp[3] ^ (tmp[0] & tmp[3]) ^ (tmp[1] & tmp[2] & tmp[3]);
}

void key_schedule(uint8_t key[10], uint16_t rk[26][4]) {
	uint16_t K[5];
	for (int i = 0; i < 5; i++) {
		K[4 - i] = key[2 * i] ^ ((uint16_t)key[2 * i + 1] << 8);
	}
	for (int i = 0; i < 4; i++) 
		rk[0][i] = K[i];
	for (int r = 1; r < 26; r++) {
		uint16_t temp[4],temp1[5];
		for (int i = 0; i < 4; i++)
			temp[i] = K[i] & 0xf;
		sbox(temp, 4);

		for (int i = 0; i < 4; i++) 
			K[i] = ((K[i] >> 4) << 4) ^ temp[i];

		for (int i = 0; i < 5; i++) 
			temp1[i] = K[i];
		K[0] = cyc_left(temp1[0], 8) ^ temp1[1] ^ RC[r - 1];
		K[1] = temp1[2];
		K[2] = temp1[3];
		K[3] = cyc_left(temp1[3], 12) ^ temp1[4];
		K[4] = temp1[0];
		for (int i = 0; i < 4; i++)
			rk[r][i] = K[i];
	}
}

void get_p(){
    int cnt = 0;
    for(int i = 0; i < 16; i++)
        for(int j = 0; j < 4; j++){
            ptable[cnt] = (i + 16*j) % 64;
            cnt++;
        }

    cnt = 0;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 16; j++){
            pitable[cnt] = (4*j + i) % 64;
            cnt++;
        }
    }

}

void get_ptable(){
    uint64_t tmp;
    uint64_t tmp_l[64], p_list[64];
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 256; j++){
            uint64_t p = 0;

            tmp = (uint64_t)j << (8 * i);
            for(int k = 0; k < 64; k++){
                tmp_l[k] = tmp & 1;
                tmp = tmp >> 1;
            }

            for(int k = 0; k < 64; k++) p_list[k] = tmp_l[ptable[k]];
            for(int k = 0; k < 64; k++) p += p_list[k] << k;

            p_table[i][j] = p;
        }
    }

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 256; j++){
            uint64_t p = 0;

            tmp = (uint64_t)j << (8 * i);
            for(int k = 0; k < 64; k++){
                tmp_l[k] = tmp & 1;
                tmp = tmp >> 1;
            }

            for(int k = 0; k < 64; k++) p_list[k] = tmp_l[pitable[k]];
            for(int k = 0; k < 64; k++) p += p_list[k] << k;

            pi_table[i][j] = p;
        }
    }

}

void cyc_left_list(uint64_t list[64], uint64_t list_left[64]){
    uint64_t p = 0;
    uint16_t p_16[4];

    for(int i = 0; i < 64; i++) p = p + (list[i] << i);

    for(int i = 0; i < 4; i++) p_16[i] = (p >> (16 * i)) & 0xffff;
    
    p_16[1] = cyc_left(p_16[1], 1);
    p_16[2] = cyc_left(p_16[2], 12);
    p_16[3] = cyc_left(p_16[3], 13);

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 16; j++){
            list_left[16*i + j] = p_16[i] & 1;
            p_16[i] = p_16[i] >> 1;
        }
    }

}

void get_stable(){
    uint64_t tmp;
    uint64_t tmp_l[64], pi_list[64];
    uint16_t testj = 0xffff, testi = 0xffff;
    for(uint64_t i = 0; i < 8; i++){
        for(uint64_t j = 0; j < 256; j++){
            uint64_t s0, s1;
            uint64_t p = 0;
            s0 = j & 0xf; s1 = (j >> 4) & 0xf;
            s0 = S[s0]; s1 = S[s1];
            tmp = (s0 << (8 * i)) ^ (s1 << (8 * i + 4));
            
            for(int k = 0; k < 64; k++){
                tmp_l[k] = tmp & 1;
                tmp = tmp >> 1;
            }

            for(int k = 0; k < 64; k++) pi_list[k] = tmp_l[pitable[k]];
            uint64_t pi_left[64], p_list[64];
            cyc_left_list(pi_list, pi_left);
            for(int k = 0; k < 64; k++) p_list[k] = pi_left[ptable[k]];
            for(int k = 0; k < 64; k++) p += p_list[k] << k;

            s_table[i][j] = p;
        }
    }
}

uint64_t look_up_stable(uint64_t a){
    uint64_t a_8[8];
    uint64_t res = 0;
    for(int i = 0; i < 8; i++) a_8[i] = (a >> (i * 8)) & 0xff;

    for(int i = 0; i < 8; i++){
        res ^= s_table[i][a_8[i]];
    }

    return res;
}

uint64_t look_up_ptable(uint64_t a){
    uint64_t a_8[8];
    uint64_t res = 0;
    for(int i = 0; i < 8; i++) a_8[i] = (a >> (i * 8)) & 0xff;

    for(int i = 0; i < 8; i++){
        res ^= p_table[i][a_8[i]];
    }

    return res;
}

uint64_t look_up_pitable(uint64_t a){
    uint64_t a_8[8];
    uint64_t res = 0;
    for(int i = 0; i < 8; i++) a_8[i] = (a >> (i * 8)) & 0xff;

    for(int i = 0; i < 8; i++){
        res ^= pi_table[i][a_8[i]];
    }

    return res;
}

void key_rearrange(uint16_t rk[26][4], uint64_t rk_64[26]){
    for(int i = 0; i < 26; i++){
        uint64_t tmp;
        rk_64[i] = 0;
        for(int j = 0; j < 4; j++){
            tmp = (uint64_t)rk[i][j];
            tmp = tmp << (16 * j);
            rk_64[i] ^= tmp;
        }
    }
}

uint64_t rectangle_encrypt(uint64_t pt, uint64_t rk_64[26]){
    uint64_t key;

    pt = pt ^ rk_64[0];
    pt = look_up_ptable(pt);
    

    for(int i = 1; i < 26; i++){
        pt = look_up_stable(pt);
        if(i == 25) break;

        key = look_up_ptable(rk_64[i]);
        pt = pt ^ key;
    }

    key = rk_64[25];
    pt = look_up_pitable(pt);
    pt = pt ^ key;

    uint64_t ct[4];
    for(int i = 0; i < 4; i++){
        ct[3 - i] = pt & 0xffff;
        ct[3 - i] = ct[3 - i] << (16 * (3 - i));
        pt = pt >> 16;
    }
    pt = ct[0] ^ ct[1] ^ ct[2] ^ ct[3];
    
    return pt;
}

int main(){
    get_p();
    get_ptable();
    get_stable();

    //uint64_t pt = 0xffffffffffffffff;
    uint64_t pt = 0x0000000000000000;
	//uint8_t key[10] = { 0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff };
    uint8_t key[10] = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
	uint64_t ct;
	uint16_t rk[26][4];
    uint64_t rk_64[26];

	key_schedule(key, rk);
    key_rearrange(rk, rk_64);
    for(int i = 0; i < 26; i++) printf("rk[%d]:%llx\n", i, rk_64[i]);
    
    ct = rectangle_encrypt(pt, rk_64);
    printf("ct:%llx", ct);

    return  0;
}