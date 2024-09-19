#include "iostream"
#include "fstream"
#include <time.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <pmmintrin.h>
#include <tmmintrin.h>
#include <smmintrin.h>
#include <wmmintrin.h>
#include <intrin.h>
using namespace std;

#define N23 8388608
// 2^(23)
#define N8 256
#pragma intrinsic(__rdtsc)

unsigned short int plaintext[N23][4];

__inline volatile unsigned long long read_tsc(void)
{
	return __rdtsc();
}

#define RAND(a, b) (((a = 36969 * (a & 65535) + (a >> 16)) << 16) + \
					(b = 18000 * (b & 65535) + (b >> 16)))

void block_rndfill(unsigned char *buf, const int len)
{
	static unsigned long a[2], mt = 1, count = 4;
	static unsigned char r[4];
	int i;

	if (mt)
	{
		mt = 0;
		*(unsigned long long *)a = read_tsc();
	}

	for (i = 0; i < len; ++i)
	{
		if (count == 4)
		{
			*(unsigned long *)r = RAND(a[0], a[1]);
			count = 0;
		}

		buf[i] = r[count++];
	}
}

void main()
{

	clock_t clock1 = clock();
	__m128i AESkey[10];
	__m128i rmm00;
	block_rndfill((unsigned char *)(AESkey), sizeof(AESkey));
	block_rndfill((unsigned char *)(&rmm00), sizeof(rmm00));
	ofstream file;
	file.open("plaintext.bin", ios_base::out | ios_base::binary);
	for (int j = 0; j < N8; j++)
	{
		for (int i = 0; i < N23; i++)
		{
			if ((i & 0x1) == 0)
			{
				rmm00 = _mm_aesenc_si128(_mm_aesenc_si128(_mm_aesenc_si128(_mm_aesenc_si128(_mm_aesenc_si128(_mm_aesenc_si128(_mm_aesenc_si128(_mm_aesenc_si128(_mm_aesenc_si128(_mm_aesenc_si128(rmm00, AESkey[0]), AESkey[1]), AESkey[2]), AESkey[3]), AESkey[4]), AESkey[5]), AESkey[6]), AESkey[7]), AESkey[8]), AESkey[9]);
			}
			unsigned long long *randnp = (unsigned long long *)(&rmm00);
			*(unsigned long long *)plaintext[i] = *(randnp + (i & 0x1));
		}
		file.write((char *)plaintext, sizeof(plaintext));
	}
	file.close();
	clock_t clock2 = clock();
	cout << "time used is " << (double)(clock2 - clock1) / CLOCKS_PER_SEC << endl;
	cout << "Done!" << endl;
	getchar();
}