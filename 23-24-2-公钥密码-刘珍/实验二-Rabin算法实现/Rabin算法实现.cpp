#include <iostream>
#include <gmpxx.h>
using namespace std;

void Rabin_key_gen(mpz_class &, mpz_class &, mpz_class &, mpz_class &);
void Rabin_Encrypt(const mpz_class &, const mpz_class &, const mpz_class &, mpz_class &);
void Rabin_Decrypt(const mpz_class &, const mpz_class &, const mpz_class &, const mpz_class &, const mpz_class &);
void ExEculid(mpz_class &, const mpz_class &, const mpz_class &);
void CRT(const mpz_class &, const mpz_class &, const mpz_class &, const mpz_class &, mpz_class &);

int main()
{
    // 密钥生成
    mpz_class p, q, n, e;
    Rabin_key_gen(p, q, n, e);

    mpz_class m, c, m_decrypt;
    m = 20;
    cout << "明文:" << endl;
    cout << m << endl;

    // 加密
    Rabin_Encrypt(n, e, m, c);
    cout << "加密结果：" << endl;
    cout << c << endl;

    // 解密
    Rabin_Decrypt(p, q, n, e, c);

    return 0;
}

void Rabin_key_gen(mpz_class &p, mpz_class &q, mpz_class &n, mpz_class &e)
{
    p = 7, q = 11;
    n = p * q;
    e = 0;
}

void Rabin_Encrypt(const mpz_class &n, const mpz_class &e, const mpz_class &m, mpz_class &c)
{
    c = (m * (m + e)) % n;
}

void Rabin_Decrypt(const mpz_class &p, const mpz_class &q, const mpz_class &n, const mpz_class &e, const mpz_class &c)
{
    mpz_class p_inv_2, q_inv_2, pow_base, p_pow, q_pow, a_1, a_2, b_1, b_2, m_1, m_2, m_3, m_4;
    
    ExEculid(p_inv_2, 2, p);
    ExEculid(q_inv_2, 2, q);
    pow_base = e * e + 4 * c;
    p_pow = (p + 1) / 4;
    q_pow = (q + 1) / 4;

    mpz_powm(a_1.get_mpz_t(), pow_base.get_mpz_t(), p_pow.get_mpz_t(), p.get_mpz_t());
    a_2 = (((p - a_1 - e) * p_inv_2) % p + p) % p;
    a_1 = (((a_1 - e) * p_inv_2) % p + p) % p;
    mpz_powm(b_1.get_mpz_t(), pow_base.get_mpz_t(), q_pow.get_mpz_t(), q.get_mpz_t());
    b_2 = (((q - b_1 - e) * q_inv_2) % q + q) % q;
    b_1 = (((b_1 - e) * q_inv_2) % q + q) % q;

    CRT(p, q, a_1, b_1, m_1);
    CRT(p, q, a_1, b_2, m_2);
    CRT(p, q, a_2, b_1, m_3);
    CRT(p, q, a_2, b_2, m_4);

    cout << "解密结果:" << endl;
    cout << m_1 << endl;
    cout << m_2 << endl;
    cout << m_3 << endl;
    cout << m_4 << endl;
}

// result = a^-1 mod b
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

// x = r_1 mod p, x = r_2 mod q, x = result mod p * q
void CRT(const mpz_class &p, const mpz_class &q, const mpz_class &r_1, const mpz_class &r_2, mpz_class &result)
{
    mpz_class p_inv, q_inv;

    ExEculid(p_inv, p, q);
    ExEculid(q_inv, q, p);

    result = (q_inv * q * r_1 + p_inv * p * r_2) % (p * q);
}