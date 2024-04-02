#include <iostream>
#include <gmpxx.h>
#include <cstdlib>
#include <ctime>
using namespace std;

void MH_Knapsack_key_gen(const mpz_class &, mpz_class *&, mpz_class *&, const int, mpz_class &, mpz_class &, mpz_class &, mpz_class &);
void ExEculid(mpz_class &, const mpz_class &, const mpz_class &);
void MH_Knapsack_Encrypt();
void MH_Knapsack_Decrypt();
void MH_Knapsack_Decrypt_ES();

int main()
{
    mpz_class m = 0B011011;
    cout << "明文:" << m << endl;
    mpz_class *A, *B, s, k, t, v;
    MH_Knapsack_key_gen(m, A, B, 1, s, k, t, v);
    


    delete[] A;
    delete[] B;

    return 0;
}

void MH_Knapsack_key_gen(const mpz_class &m, mpz_class *&A, mpz_class *&B, const int Knapsack_length_add, mpz_class &s, mpz_class &k, mpz_class &t, mpz_class &v)
{
    srand(time(0));

    // 背包元素数量
    int Knapsack_length = mpz_sizeinbase(m.get_mpz_t(), 2) + Knapsack_length_add;
    A = new mpz_class[Knapsack_length];
    B = new mpz_class[Knapsack_length];

    s = 0;

    // 随机生成超递增背包
    A[0] = rand();
    for (int i = 1; i < Knapsack_length; i++)
    {
        s += A[i - 1];
        A[i] = s + rand();
    }
    
    k = 105, t = 31;
    ExEculid(v, t, k);

    for (int i = 0; i < Knapsack_length; i++)
        B[i] = (t * A[i]) % k;
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