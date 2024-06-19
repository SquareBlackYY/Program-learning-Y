#include <iostream>
#include <gmpxx.h>
#include <cstdlib>
#include <ctime>
#include <chrono>
using namespace std;


void RSA_key_gen(mpz_class &, mpz_class &, mpz_class &, mpz_class &, mpz_class &, mpz_class &);
void RSA_Encrypt(const mpz_class &, mpz_class &, const mpz_class &, const mpz_class &);
void RSA_Decrypt(const mpz_class &, mpz_class &, const mpz_class &, const mpz_class &);
void ExEculid(mpz_class &, const mpz_class &, const mpz_class &);
void ExEculid2(mpz_class &, mpz_class &, const mpz_class &, const mpz_class &);
void CRT(const mpz_class &, const mpz_class &, const mpz_class &, const mpz_class &, mpz_class &);
void quick_pow_mod(mpz_class, mpz_class, const mpz_class &, mpz_class &);
bool Miller_Rabin(const mpz_class &);
void RSA_FastDecrypt(const mpz_class &, mpz_class &, const mpz_class &, const mpz_class &, const mpz_class &);
mpz_class generate_random_number(const mpz_class &, const mpz_class &);
bool GCD(const mpz_class &, const mpz_class &);
mpz_class RSA_common_modulus_attack(mpz_class, mpz_class, const mpz_class &, const mpz_class &, const mpz_class &);
mpz_class RSA_low_exponent_attack(const mpz_class &, const mpz_class &, const mpz_class &);

int main()
{
    cout << "(1) 密钥生成，要求用Miller-Rabin算法进行素性检测，随机生成p和q的长度大约为512比特" << endl;
    mpz_class p, q, n, fn, e, d;
    RSA_key_gen(p, q, n, fn, e, d);
    cout << "p = " << p << endl;
    cout << "q = " << q << endl;


    cout << "(2) 加密，要求分别利用快速指数模幂运算与一般模幂运算实现加密过程，分别运行加密过程200次，并列出运行时间对比，单位为ms" << endl;
    mpz_class m, c;
    m = rand();
    chrono::duration<double, milli> duration;
    
    // 快速指数模幂运算
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < 200; i++)
    {
        quick_pow_mod(m, e, n, c);
    }
    auto end = chrono::high_resolution_clock::now();
    duration = end - start;
    cout << "快速指数运算时间: " << duration.count() << " ms" << endl;

    // // 一般模幂运算
    // start = chrono::high_resolution_clock::now();
    // for (int i = 0; i < 200; i++)
    // {
    //     c = m;
    //     for (mpz_class i = 1; i < e; i++)
    //         c *= m;
    //     c % n;
    // }
    // end = chrono::high_resolution_clock::now();
    // duration = end - start;
    // cout << "一般模幂运算时间: " << duration.count() << " ms" << endl;


    cout << "(3) 解密，要求使用中国剩余定理，快速指数模幂运算加速" << endl;
    mpz_class m_decrypt;

    m = 513;
    cout << "明文 m = " << m << endl;

    RSA_Encrypt(m, c, e, n);
    cout << "加密结果 c = " << c << endl;

    RSA_FastDecrypt(c, m_decrypt, d, p, q);
    cout << "解密结果 m_decrypt = " << m_decrypt << endl;


    cout << "(4) 实现RSA的共模攻击" << endl;
    cout << "明文 m = " << m << endl;
    
    mpz_class e1, e2, c1, c2;
    do
    {
        e1 = generate_random_number(3, fn);
        e2 = generate_random_number(3, fn);
    } while (!GCD(e1, fn) || !GCD(e2, fn) || !GCD(e1, e2));

    RSA_Encrypt(m, c1, e1, n);
    cout << "加密结果1 c1 = " << c1 << endl;
    RSA_Encrypt(m, c2, e2, n);
    cout << "加密结果2 c2 = " << c2 << endl;

    mpz_class m_common_modulus_attack = RSA_common_modulus_attack(c1, c2, e1, e2, n);
    cout << "共模攻击结果 m = " << m_common_modulus_attack << endl;


    cout << "(5) 实现RSA的低加密指数攻击" << endl;
    cout << "明文 m = " << m << endl;

    e = 3;
    cout << "低加密指数 e = " << e << endl;

    RSA_Encrypt(m, c, e, n);
    cout << "加密结果 c = " << c << endl;

    mpz_class m_low_exponent_attack = RSA_low_exponent_attack(c, e, n);
    cout << "低加密指数攻击结果 m = " << m_low_exponent_attack << endl;

    return 0;
}

// 密钥生成
void RSA_key_gen(mpz_class &p, mpz_class &q, mpz_class &n, mpz_class &fn, mpz_class &e, mpz_class &d)
{
    srand(time(0));
    // 生成p, q
    int bit_length = 512;
    do
    {
        p = 1;
        for (int i = 1; i < bit_length; i++)
            p = (p << 1) + (rand() % 2);
    } while (!Miller_Rabin(p));
    do
    {
        q = 1;
        for (int i = 1; i < bit_length; i++)
            q = (q << 1) + (rand() % 2);
    } while (!Miller_Rabin(q));

    n = p * q;
    fn = (p - 1) * (q - 1);

    // 生成公钥
    e = 65537;

    // 计算私钥
    ExEculid(d, e, fn);
}

// 快速指数幂模运算
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

// 扩展欧里几得算法求模逆 result = a^-1 mod b
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

// 扩展欧里几得算法求 s * a + t * b = 1
void ExEculid2(mpz_class &s, mpz_class &t, const mpz_class &a, const mpz_class &b)
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

    s = x0;
    t = y0;
}

// 中国剩余定理 x = r_1 mod p, x = r_2 mod q, x = result mod p * q
void CRT(const mpz_class &p, const mpz_class &q, const mpz_class &r_1, const mpz_class &r_2, mpz_class &result)
{
    mpz_class p_inv, q_inv;

    ExEculid(p_inv, p, q);
    ExEculid(q_inv, q, p);

    result = (q_inv * q * r_1 + p_inv * p * r_2) % (p * q);
}

// RSA解密快速实现
void RSA_FastDecrypt(const mpz_class &c, mpz_class &m, const mpz_class &d, const mpz_class &p, const mpz_class &q)
{
    mpz_class v_p, v_q;
    
    mpz_powm(v_p.get_mpz_t(), c.get_mpz_t(), d.get_mpz_t(), p.get_mpz_t());
    mpz_powm(v_q.get_mpz_t(), c.get_mpz_t(), d.get_mpz_t(), q.get_mpz_t());
    
    CRT(p, q, v_p, v_q, m);
}

// 实现Miller-Rabin算法
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

    // 执行轮数
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

// 生成随机数（前闭后开）
mpz_class generate_random_number(const mpz_class &lowerBound, const mpz_class &upperBound)
{
    gmp_randclass rand(gmp_randinit_mt);
    static bool seed_initialized = false;
    if (!seed_initialized)
    {
        rand.seed(time(0));
        seed_initialized = true;
    }
    return lowerBound + rand.get_z_range(upperBound - lowerBound);
}

// 判断两个数是否互素
bool GCD(const mpz_class& a, const mpz_class& b)
{
    mpz_class gcd_result;
    mpz_gcd(gcd_result.get_mpz_t(), a.get_mpz_t(), b.get_mpz_t());
    return gcd_result == 1;
}

// RSA共模攻击
mpz_class RSA_common_modulus_attack(mpz_class c1, mpz_class c2, const mpz_class &e1, const mpz_class &e2, const mpz_class &n)
{
    mpz_class s, t, c1_inv, c2_inv;
    ExEculid2(s, t, e1, e2);

    ExEculid(c1_inv, c1, n);
    ExEculid(c2_inv, c2, n);

    mpz_class result1, result2;
    mpz_powm(result1.get_mpz_t(), c1.get_mpz_t(), s.get_mpz_t(), n.get_mpz_t());
    mpz_powm(result2.get_mpz_t(), c2.get_mpz_t(), t.get_mpz_t(), n.get_mpz_t());
    return (result1 * result2) % n;
}

// RSA低加密指数攻击
mpz_class RSA_low_exponent_attack(const mpz_class &c, const mpz_class &e, const mpz_class &n)
{
    mpz_class k = 0, m, result;
    bool exact_root = false;

    while (!exact_root)
    {
        mpz_class candidate = c + k * n;
        exact_root = mpz_root(m.get_mpz_t(), candidate.get_mpz_t(), e.get_ui());
        if (exact_root)
            return m;
        k++;
    }
    return 0;
}
