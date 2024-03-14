#include <iostream>
#include <gmpxx.h> //编译时添加参数-lgmp -lgmpxx
#include <cmath>
using namespace std;

void ExEuclid(mpz_class &, const mpz_class &, const mpz_class &);
void RSA_encrypt(const mpz_class &, mpz_class &, const mpz_class &, const mpz_class &);
void RSA_decrypt(const mpz_class &, mpz_class &, const mpz_class &, const mpz_class &, const mpz_class &, const mpz_class &);

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

    mpz_class m, c, m_decrypt;
    m = 513;

    // 加密
    RSA_encrypt(m, c, e, n);

    // 输出加密结果
    cout << c << endl;

    // 解密
    RSA_decrypt(c, m_decrypt, d, e, p, q);

    // 输出解密结果
    cout << m_decrypt << endl;

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

// RSA加密 明文m 密文c 公钥e 大整数n
void RSA_encrypt(const mpz_class &m, mpz_class &c, const mpz_class &e, const mpz_class &n)
{
    mpz_powm(c.get_mpz_t(), m.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());
}

// RSA解密 密文c 解密结果m 私钥d 公钥e 整数分解p,q
void RSA_decrypt(const mpz_class &c, mpz_class &m, const mpz_class &d, const mpz_class &e, const mpz_class &p, const mpz_class &q)
{
    mpz_class q_inv, dp, dq, m1, m2, h, m_abs;
    ExEuclid(q_inv, q, p);

    ExEuclid(dp, e, p - 1);
    ExEuclid(dq, e, q - 1);

    mpz_powm(m1.get_mpz_t(), c.get_mpz_t(), dp.get_mpz_t(), p.get_mpz_t());
    mpz_powm(m2.get_mpz_t(), c.get_mpz_t(), dq.get_mpz_t(), q.get_mpz_t());

    h = q_inv * (m1 - m2) % p;
    if (h < 0)
        h += p;

    m = m2 + h * q;
}
