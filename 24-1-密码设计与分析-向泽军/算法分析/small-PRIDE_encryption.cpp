#include "iostream"
#include "time.h"
#include "cstdlib"
#include "fstream"
using namespace std;
#define ROUND 9

unsigned short int RC[ROUND + 1][4] = {
	/*RC0 */ {
		0x0000,
		0x0000,
		0x0000,
		0x0000,
	},
	/*RC1 */ {
		0x7344,
		0x0370,
		0x8a2e,
		0x1319,
	},
	/*RC2 */ {
		0x31d0,
		0x299f,
		0x3822,
		0xa409,
	},
	/*RC3 */ {
		0x6c89,
		0xec4e,
		0xfa98,
		0x082e,
	},
	/*RC4 */ {
		0x1377,
		0x38d0,
		0x21e6,
		0x4528,
	},
	/*RC5 */ {
		0x0c6c,
		0x34e9,
		0x66cf,
		0xbe54,
	},
	/*RC6 */ {
		0x5cb1,
		0xfd95,
		0x4f78,
		0x7ef8,
	},
	/*RC7 */ {
		0x43aa,
		0xf1ac,
		0x0851,
		0x8584,
	},
	/*RC8 */ {
		0x3c54,
		0x2532,
		0xd32f,
		0xc882,
	},
	/*RC9 */ {
		0x0000,
		0x0000,
		0x0000,
		0x0000,
	}};
//**************************
// the state of the PRIDE is arranged into a unsigned short int array.
// the first(second, third, froth) row of the state is left multipled by a matrix0(1,2,3,respectively) in the linear layer.
// each column of the state is subsitute by a fixed permutatoin which is called Sbox.

// matrix multiplication of the first row of the state
unsigned short int matrix0(unsigned short int state0)
{
	state0 = (state0 << 4) ^ (state0 >> 12) ^ (state0 << 8) ^ (state0 >> 8) ^ (state0 << 12) ^ (state0 >> 4);
	return state0;
}
// matrix multiplication of the second row of the state
unsigned short matrix1(unsigned short int state1)
{
	unsigned char state[2];
	unsigned char a, b;
	state[0] = (state1 >> 8);
	state[1] = (state1 & 0xff);
	a = state[0] ^ (state[0] << 1) ^ (state[0] >> 7) ^ (state[1] << 3) ^ (state[1] >> 5);
	b = state[0] ^ (state[1] << 3) ^ (state[1] >> 5) ^ (state[1] << 4) ^ (state[1] >> 4);
	state1 = a * 256 + b;
	return state1;
}
// matrix multiplication of the third row of the state
unsigned short int matrix2(unsigned short int state2)
{
	unsigned char state[2];
	unsigned char a, b;
	state[0] = (state2 >> 8);
	state[1] = state2 & 0xff;
	a = (state[0] << 4) ^ (state[0] >> 4) ^ (state[0] << 5) ^ (state[0] >> 3) ^ (state[1] << 7) ^ (state[1] >> 1);
	b = (state[0] << 4) ^ (state[0] >> 4) ^ state[1] ^ (state[1] << 7) ^ (state[1] >> 1);
	state2 = a * 256 + b;
	return state2;
}
// matrix multiplication of the forth row of the state
unsigned short int matrix3(unsigned short int state3)
{
	state3 = state3 ^ (state3 << 4) ^ (state3 >> 12) ^ (state3 << 12) ^ (state3 >> 4);
	return state3;
}
// Sbox layer of PRIDE
void Sbox(unsigned short int state[4])
{
	unsigned short int temp1, temp2;
	temp1 = state[0];
	temp2 = state[1];
	state[0] = state[2] ^ (state[0] & state[1]);
	state[1] = state[3] ^ (state[1] & state[2]);
	state[2] = temp1 ^ (state[0] & state[1]);
	state[3] = temp2 ^ (state[1] & state[2]);
}

// generate the 10 round keys Round_key[10][4] by the master key Key[4]
void Key_schedule(unsigned short int Key[4], unsigned short int Round_key[ROUND + 1][4])
{
	for (int i = 0; i < ROUND + 1; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Round_key[i][j] = Key[j] ^ RC[i][j];
		}
	}
}
// one round encryption function of PRIDE
void One_round_encryption(unsigned short int state[4], unsigned short int round_key[4])
{
	for (int i = 0; i < 4; i++)
		state[i] = state[i] ^ round_key[i];
	Sbox(state);
	state[0] = matrix0(state[0]);
	state[1] = matrix1(state[1]);
	state[2] = matrix2(state[2]);
	state[3] = matrix3(state[3]);
}
// last round encryption function of PRIDE
void Last_round_encryption(unsigned short int state[4], unsigned short int round_key[4])
{
	for (int i = 0; i < 4; i++)
	{
		state[i] = state[i] ^ round_key[i];
	}
	Sbox(state);
}
// one block PRIDE encryption
void small_PRIDE_encryption_with_keyschedule(unsigned short int state[4], unsigned short int Key[4])
{
	unsigned short int Round_key[ROUND + 1][4];
	Key_schedule(Key, Round_key);
	for (int i = 0; i < ROUND - 1; i++)
		One_round_encryption(state, Round_key[i]);
	Last_round_encryption(state, Round_key[ROUND - 1]);
	for (int i = 0; i < 4; i++)
	{
		state[i] = state[i] ^ Round_key[ROUND][i];
	}
}

void small_PRIDE_encryption_without_keyschedule(unsigned short int state[4], unsigned short int Round_key[ROUND + 1][4])
{
	for (int i = 0; i < ROUND - 1; i++)
		One_round_encryption(state, Round_key[i]);
	Last_round_encryption(state, Round_key[ROUND - 1]);
	for (int i = 0; i < 4; i++)
	{
		state[i] = state[i] ^ Round_key[ROUND][i];
	}
}

int main()
{
	unsigned short int Key[4] = {0x0F9D, 0x4ACD, 0xA5D6, 0x0081};
	unsigned short int Round_key[ROUND + 1][4];
	unsigned short int state[4] = {0x585f, 0x1dd0, 0xfa8a, 0xb3f8};
	small_PRIDE_encryption_with_keyschedule(state, Key);
	for (int i = 0; i < 4; i++)
		cout << hex << state[i] << endl;
	return 0;
}