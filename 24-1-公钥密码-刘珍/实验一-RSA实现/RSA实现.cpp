#include <iostream>
#include <gmpxx.h>
using namespace std;

// （1）RSA基本实现
void RSA_key_gen(mpz_class &, mpz_class &, mpz_class &, mpz_class &, mpz_class &);
void RSA_Encrypt(const mpz_class &, mpz_class &, const mpz_class &, const mpz_class &);
void RSA_Decrypt(const mpz_class &, mpz_class &, const mpz_class &, const mpz_class &);

// （2）实现扩展的欧几里得算法
void ExEculid(mpz_class &, const mpz_class &, const mpz_class &);

// （3）实现中国剩余定理（实现在（6）RSA快速实现中）

// （4）实现快速指数幂模运算
void quick_pow_mod_1(mpz_class, mpz_class, const mpz_class &, mpz_class &);
void quick_pow_mod_2();

// （5）实现Miller-Rabin算法
bool Miller_Rabin(const mpz_class &);

// （6）RSA快速实现
void RSA_FastDecrypt(const mpz_class &, mpz_class &, const mpz_class &, const mpz_class &, const mpz_class &, const mpz_class &);

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

    mpz_class m, c, m_decrypt;
    m = 513;

    // 加密
    RSA_Encrypt(m, c, e, n);
    cout << c << endl;

    // 解密
    RSA_Decrypt(c, m_decrypt, d, n);
    cout << m_decrypt << endl;

    // 快速解密
    RSA_FastDecrypt(c, m_decrypt, d, e, p, q);
    cout << m_decrypt << endl;

    return 0;
}

// （1）RSA简单实现
// 密钥生成
void RSA_key_gen(mpz_class &p, mpz_class &q, mpz_class &n, mpz_class &e, mpz_class &d)
{
    p = 137, q = 131;
    n = p * q;
    mpz_class fn = (p - 1) * (q - 1);
    e = 3;
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

// 快速幂取模
void quick_pow_mod_1(mpz_class n, mpz_class power, const mpz_class &mod, mpz_class &result)
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
void quick_pow_mod_2()
{
    
}

// Miller-Rabin素性检测
bool Miller_Rabin(const mpz_class &n)
{
    if (n < 2)
        return false;
    if (n == 2)
        return true;
    if (n % 2 == 0)
        return false;

    mpz_class s = 0, d = n - 1;
    while (d % 2 == 0)
    {
        s++;
        d /= 2;
    }

    // 5轮测试
    for (int i = 0; i < 5; i++)
    {
        mpz_class a = rand() % (n - 1) + 1, x;
        quick_pow_mod_1(a, d, n, x);
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

// （6）RSA解密快速实现 使用扩展欧里几德算法及（3）中国剩余定理
void RSA_FastDecrypt(const mpz_class &c, mpz_class &m, const mpz_class &d, const mpz_class &e, const mpz_class &p, const mpz_class &q)
{
    mpz_class q_inv, dp, dq, m1, m2, h;
    ExEculid(q_inv, q, p);

    ExEculid(dp, e, p - 1);
    ExEculid(dq, e, q - 1);

    mpz_powm(m1.get_mpz_t(), c.get_mpz_t(), dp.get_mpz_t(), p.get_mpz_t());
    mpz_powm(m2.get_mpz_t(), c.get_mpz_t(), dq.get_mpz_t(), q.get_mpz_t());

    h = q_inv * (m1 - m2) % p;
    if (h < 0)
        h += p;

    m = m2 + h * q;
}

//（7）RSA-OAEP
// 掩码生成
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