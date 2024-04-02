#include <iostream>
#include <gmpxx.h>
#include <vector>
#include <bitset>
#include <cstdlib>
#include <ctime>
using namespace std;

void MH_Knapsack_key_gen(const int, vector<mpz_class> &, vector<mpz_class> &, mpz_class &, mpz_class &, mpz_class &, mpz_class &);
void MH_Knapsack_Encrypt(const int, const mpz_class &, const vector<mpz_class> &, mpz_class &);
void MH_Knapsack_Decrypt(const int, const mpz_class &, const mpz_class &, const mpz_class &, const vector<mpz_class> &, mpz_class &);
void MH_Knapsack_Decrypt_ES(const int, const mpz_class &, const vector<mpz_class> &, mpz_class &);
void ExEculid(mpz_class &, const mpz_class &, const mpz_class &);

int main()
{
    mpz_class m = 0b011011;
    cout << "明文:" << m << endl;

    vector<mpz_class> A, B;
    mpz_class s, k, t, v;
    int Knapsack_length = 6;
    MH_Knapsack_key_gen(Knapsack_length, A, B, s, k, t, v);
    
    mpz_class c;
    MH_Knapsack_Encrypt(Knapsack_length, m, B, c);
    cout << "加密结果:" << c << endl;

    mpz_class m_decrypt;
    MH_Knapsack_Decrypt(Knapsack_length, k, v, c, A, m_decrypt);
    cout << "解密结果:" << m_decrypt << endl;

    MH_Knapsack_Decrypt_ES(Knapsack_length, c, B, m_decrypt);
    cout << "穷搜法解密结果:" << m_decrypt << endl;

    return 0;
}

void MH_Knapsack_key_gen(const int Knapsack_length, vector<mpz_class> &A, vector<mpz_class> &B, mpz_class &s, mpz_class &k, mpz_class &t, mpz_class &v)
{
    s = 0;

    A.insert(A.end(), {2, 3, 6, 13, 27, 52});
    
    k = 105, t = 31;
    ExEculid(v, t, k);

    for (int i = 0; i < Knapsack_length; i++)
        B.push_back((t * A[i]) % k);
}

void MH_Knapsack_Encrypt(const int Knapsack_length, const mpz_class &m, const vector<mpz_class> &B, mpz_class &c)
{
    c = 0;
    for (int i = 0; i < Knapsack_length; i++)
    {
        c += ((m >> i) & 1) * B[Knapsack_length - i - 1];
    }
}

void MH_Knapsack_Decrypt(const int Knapsack_length, const mpz_class &k, const mpz_class &v, const mpz_class &c, const vector<mpz_class> &A, mpz_class &m)
{
    m = 0;
    int bit;
    mpz_class s = (v * c) % k;
    for (int i = Knapsack_length - 1; s != 0; i--)
        if (s - A[i] >= 0)
        {
            m += 1 << (Knapsack_length - i - 1);
            s -= A[i];
        }
}


void MH_Knapsack_Decrypt_ES(const int Knapsack_length, const mpz_class &c, const vector<mpz_class> &B, mpz_class &m)
{
    int Max = 1 << Knapsack_length;
    mpz_class result = 0;
    for (m = 0; m < Max && result != c; m++)
    {
        result = 0;
        for (int i = 0; i < Knapsack_length; i++)
            result += ((m >> i) & 1) * B[Knapsack_length - i - 1];
    }
    m--;
}

void ExEculid(mpz_class &result, const mpz_class &a, const mpz_class &b)
{
    mpz_class a_copy = a;
    mpz_class b_copy = b;
    
    mpz_class x0 = 1, y0 = 0, x1 = 0, y1 = 1;
    mpz_class q, r, x, y;

    while (b_copy != 0)
    {
        q = a_copy / b_copy;
        r = a_copy % b_copy;

        x = x0 - q * x1;
        y = y0 - q * y1;

        a_copy = b_copy;
        b_copy = r;
        x0 = x1;
        y0 = y1;
        x1 = x;
        y1 = y;
    }

    result = x0;
    if (result < 0)
        result += b;
}