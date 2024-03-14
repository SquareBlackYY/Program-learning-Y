#include <iostream>
#include <gmpxx.h>
using namespace std;

#define length 1

void ExEuclid(mpz_class &, const mpz_class &, const mpz_class &);
void RSA_encrypt(mpz_class[], mpz_class[], mpz_class, mpz_class);
void RSA_decrypt(mpz_class[], mpz_class, mpz_class, mpz_class, mpz_class);

int main()
{
    mpz_class p(137);
    mpz_class q(131);

    mpz_class n = p * q;
    mpz_class fn = (p - 1)  * (q - 1);

    mpz_class e(3);
    mpz_class d;

    // 扩展欧几里得算法求模逆
    ExEuclid(d, e, fn);

    
    mpz_class m[length], c[length];
    m[0] = 513;

    // 加密
    RSA_encrypt(m, c, e, n);

    // 输出加密结果
    for (int i = 0; i < length; i++)
        cout << c[i] << " ";
    cout << endl;

    // 解密
    RSA_decrypt(c, d, e, p, q);
    

    return 0;
}

// 扩展到欧里几得算法求模逆 result = a^-1 mod b
void ExEuclid(mpz_class &result, const mpz_class &a, const mpz_class &b)
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

void RSA_encrypt(mpz_class m[], mpz_class c[], mpz_class e, mpz_class n)
{
    for (int i = 0; i < length; i++)
        c[i] = mpz_powm(m[i].get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());
}

void RSA_decrypt(mpz_class c[], mpz_class d, mpz_class e, mpz_class p, mpz_class q)
{
    mpz_class q_inv, dp, dq, m1, m2, h;
    ExEuclid(q_inv, p, q);

    dp = p - 1;
    dq = q - 1;

    ExEuclid(dp, dp, e);
    ExEuclid(dq, dq, e);

    for (int i = 0; i < length; i++)
    {
        mpz_class m1 = mpz_powm(c[i].get_mpz_t(), dp.get_mpz_t(), p.get_mpz_t());
        mpz_class m2 = mpz_powm(c[i].get_mpz_t(), dq.get_mpz_t(), q.get_mpz_t());

        h = m1 - m2;
        h = abs(h);
        h = h * q_inv % p;

        h = h * q;
        m[i] = m2 + h;
    }

    for (int i = 0; i < length; i++)
        cout << m[i] << " ";
    cout << endl;
}