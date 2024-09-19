#include <iostream>
#include <gmpxx.h>
using namespace std;

int main()
{
    mpz_class n("2");
    mpz_class power("10");
    mpz_class mod("1000");
    mpz_class result = 1;
    mpz_class bit;
    int length = mpz_sizeinbase(power.get_mpz_t(), 2);
    for (int i = 0; i < length; i++)
    {
        bit = (power >> i) & 1;
        if (bit)
            result = (result * n) % mod;
        n = (n * n) % mod;
    }
    cout << "结果:" << result << endl;
}   