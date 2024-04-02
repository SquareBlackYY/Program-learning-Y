#include <iostream>
#include <gmpxx.h>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

void MH_Knapsack_key_gen(const mpz_class &, vector<mpz_class> &, vector<mpz_class> &, const int);

int main()
{
    mpz_class m = 0B011011;
    cout << m << endl;
    mpz_class *A, *B;
    MH_Knapsack_key_gen(m, A, B, 2);
}

void MH_Knapsack_key_gen(const mpz_class &m, mpz_class *&A, mpz_class *&B, const int A_length_add)
{
    srand(time(0));

    // 超递增背包A元素数量
    int A_length = mpz_sizeinbase(m.get_mpz_t(), 2) + A_length_add;
    A = new mpz_class[A_length];

    // A中元素的和
    mpz_class s = 0;
    A[0] = rand();
    for (int i = 1; i < A_length; i++)
    {
        s += A[i - 1];
        A[i] = s + rand();
    }
    
    cout << A << endl;
}