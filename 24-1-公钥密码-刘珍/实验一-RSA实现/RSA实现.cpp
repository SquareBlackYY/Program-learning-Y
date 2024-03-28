#include <iostream>
#include <gmpxx.h>
#include <cstdlib>
#include <ctime>
using namespace std;

// （1）RSA基本实现
void RSA_key_gen(mpz_class &, mpz_class &, mpz_class &, mpz_class &, mpz_class &);
void RSA_Encrypt(const mpz_class &, mpz_class &, const mpz_class &, const mpz_class &);
void RSA_Decrypt(const mpz_class &, mpz_class &, const mpz_class &, const mpz_class &);

// （2）实现扩展的欧几里得算法
void ExEculid(mpz_class &, const mpz_class &, const mpz_class &);

// （3）实现中国剩余定理
void CRT(const mpz_class &, const mpz_class &, const mpz_class &, const mpz_class &, mpz_class &);

// （4）实现快速指数幂模运算
void quick_pow_mod(mpz_class, mpz_class, const mpz_class &, mpz_class &);

// （5）实现Miller-Rabin算法
bool Miller_Rabin(const mpz_class &);

// （6）RSA快速实现
void RSA_FastDecrypt(const mpz_class &, mpz_class &, const mpz_class &, const mpz_class &, const mpz_class &);

// （7）RSA-OAEP
void MGF();
void RSA_OAEP_Encode();
void RSA_OAEP_Decode();
void RSA_OAEP_Encrypt();
void RSA_OAEP_Decrypt();

int main()
{
    // 密钥生成
    mpz_class p, q, n, e, d;
    RSA_key_gen(p, q, n, e, d);
    cout << "p = " << p << endl;
    cout << "q = " << q << endl;
    cout << "e = " << e << endl;
    cout << "d = " << d << endl;

    mpz_class m, c, m_decrypt;
    m = 513;
    cout << "明文:" << m << endl;

    // 加密
    RSA_Encrypt(m, c, e, n);
    cout << "密文:" << c << endl;

    // 解密
    RSA_Decrypt(c, m_decrypt, d, n);
    cout << "解密结果:" << m_decrypt << endl;

    // 快速解密
    RSA_FastDecrypt(c, m_decrypt, d, p, q);
    cout << "快速解密结果:" << m_decrypt << endl;

    return 0;
}

// （1）RSA简单实现
// 密钥生成
void RSA_key_gen(mpz_class &p, mpz_class &q, mpz_class &n, mpz_class &e, mpz_class &d)
{
    srand(time(0));
    // 生成p, q
    int bit_length = 512;
    int bit_length_diff = 16;

    do
    {
        p = 1;
        for (int i = 1; i < bit_length; i++)
            p = (p << 1) + (rand() % 2);
    } while (!Miller_Rabin(p));

    do
    {
        q = 0;
        for (int i = 1; i < bit_length; i++)
            q = (q << 1) + (rand() % 2);
    } while (!Miller_Rabin(q) || abs((int)(bit_length - mpz_sizeinbase(q.get_mpz_t(), 2))) > bit_length_diff || p == q);

    n = p * q;
    mpz_class fn = (p - 1) * (q - 1);

    // 生成公钥
    e = 65537;

    // 计算私钥
    ExEculid(d, e, fn);
}
// RSA简单加密
void RSA_Encrypt(const mpz_class &m, mpz_class &c, const mpz_class &e, const mpz_class &n)
{
    mpz_powm(c.get_mpz_t(), m.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());
}
// RSA简单解密
void RSA_Decrypt(const mpz_class &c, mpz_class &m, const mpz_class &d, const mpz_class &n)
{
    mpz_powm(m.get_mpz_t(), c.get_mpz_t(), d.get_mpz_t(), n.get_mpz_t());
}

// （2）扩展欧里几得算法求模逆 result = a^-1 mod b
void ExEculid(mpz_class &result, const mpz_class &a, const mpz_class &b)
{
    mpz_class a_copy = a, b_copy = b;
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

// （3）实现中国剩余定理 x = r_1 mod p, x = r_2 mod q, x = result mod p * q
void CRT(const mpz_class &p, const mpz_class &q, const mpz_class &r_1, const mpz_class &r_2, mpz_class &result)
{
    mpz_class p_inv, q_inv;

    ExEculid(p_inv, p, q);
    ExEculid(q_inv, q, p);

    result = (q_inv * q * r_1 + p_inv * p * r_2) % (p * q);
}

// （4）实现快速指数幂模运算
void quick_pow_mod(mpz_class n, mpz_class power, const mpz_class &mod, mpz_class &result)
{
    result = 1;
    n %= mod;
    while (power > 0)
    {
        if (mpz_odd_p(power.get_mpz_t()))
            result = (result * n) % mod;
        n = (n * n) % mod;
        mpz_fdiv_q_2exp(power.get_mpz_t(), power.get_mpz_t(), 1);
    }
}

// （5）实现Miller-Rabin算法
bool Miller_Rabin(const mpz_class &n)
{
    if (n < 2)
        return false;
    if (n == 2)
        return true;
    if (n % 2 == 0)
        return false;

    mpz_class s = 0, k = n - 1;
    while (k % 2 == 0)
    {
        s++;
        k /= 2;
    }

    int round = 5;
    for (int i = 0; i < round; i++)
    {
        mpz_class a = rand() % (n - 1) + 1, x;
        quick_pow_mod(a, k, n, x);
        if (x == 1 || x == n - 1)
            continue;
        for (int j = 1; j < s; j++)
        {
            x = x * x % n;
            if (x == 1)
                return false;
            if (x == n - 1)
                break;
        }
        if (x != n - 1)
            return false;
    }

    return true;
}

// （6）RSA解密快速实现
void RSA_FastDecrypt(const mpz_class &c, mpz_class &m, const mpz_class &d, const mpz_class &p, const mpz_class &q)
{
    mpz_class v_p, v_q;

    mpz_powm(v_p.get_mpz_t(), c.get_mpz_t(), d.get_mpz_t(), p.get_mpz_t());
    mpz_powm(v_q.get_mpz_t(), c.get_mpz_t(), d.get_mpz_t(), q.get_mpz_t());

    CRT(m, p, q, v_p, v_q);
}

// （7）RSA-OAEP
//  掩码生成
void MGF()
{
}
// 编码
void RSA_OAEP_Encode()
{
}
// 解码
void RSA_OAEP_Decode()
{
}
// 加密
void RSA_OAEP_Encrypt()
{
}
// 解密
void RSA_OAEP_Decrypt()
{
}