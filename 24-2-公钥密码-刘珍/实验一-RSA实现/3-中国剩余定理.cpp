#include <iostream>
#include <gmpxx.h>
using namespace std;

#define length 1

void ExEuclid(mpz_class &, mpz_class &, mpz_class &);
void RSA_encrypt(mpz_class[], mpz_class[], mpz_class, mpz_class);
void RSA_decrypt(mpz_class[], mpz_class, mpz_class, mpz_class, mpz_class);

int main()
{
    mpz_class p(137);
    mpz_class q(131);
    mpz_class n = p * q;

    mpz_class p_1 = p - 1;
    mpz_class q_1 = q - 1;
    mpz_class fn = p_1 * q_1;

    mpz_class e(3);
    mpz_class d;

    // 扩展欧几里得算法求模逆
    ExEuclid(d, fn, e);

    cout << d << endl;
    /*
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
    */

    return 0;
}

void ExEuclid(mpz_class &result, mpz_class &f, mpz_class &d)
{
    mpz_class a1 = 1, b1 = 0, a2 = 0, b2 = 1;
    mpz_class q, r, x, y;

    while (d != 0)
    {
        q = f / d;

        r = f % d;
        f = d;
        d = r;

        x = a2;
        y = b2;

        a2 = a1 - q * a2;
        b2 = b1 - q * b2;

        a1 = x;
        b1 = y;
    }

    result = (a1 % f + f) % f;
}
/*
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
*/