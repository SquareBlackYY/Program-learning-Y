#include <iostream>
#include <gmpxx.h>
#include <vector>

struct ElGamal_Public_Key
{
    mpz_class p;
    mpz_class g;
    mpz_class beta;

    ElGamal_Public_Key(const mpz_class p, const mpz_class g, const mpz_class beta) : p(p), g(g), beta(beta) {}
};

struct ElGamal_Private_Key
{
    mpz_class alpha;

    ElGamal_Private_Key(const mpz_class alpha) : alpha(alpha) {}
};

struct ElGamal_ciphertext
{
    mpz_class r;
    mpz_class s;
};

mpz_class generate_random_number(const mpz_class &, const mpz_class &);
mpz_class power_mod(const mpz_class &, const mpz_class &, const mpz_class &);
mpz_class ExEculid(const mpz_class &, const mpz_class &);
ElGamal_ciphertext ElGamal_Encrypt(const mpz_class &, const ElGamal_Public_Key &);
mpz_class ElGamal_Decrypt(const ElGamal_ciphertext &, const ElGamal_Public_Key &, const ElGamal_Private_Key &);
bool is_primitive_root(const mpz_class &, const mpz_class &, const std::vector<mpz_class> &);
mpz_class generate_primitive_root(const mpz_class &);

class ElGamal_Key_gen
{
private:
    mpz_class p;
    mpz_class g;
    mpz_class alpha;
    mpz_class beta;

public:
    ElGamal_Key_gen(const mpz_class &p_input, const mpz_class &g_input = 0) : p(p_input), g(g_input)
    {
        if (g == 0)
            g = generate_primitive_root(p);
        alpha = generate_random_number(2, p - 1);
        beta = power_mod(g, alpha, p);
    }

    ElGamal_Public_Key get_public_key()
    {
        return ElGamal_Public_Key(p, g, beta);
    }
    ElGamal_Private_Key get_private_key()
    {
        return ElGamal_Private_Key(alpha);
    }
};

int main()
{
    // 示例
    mpz_class p("114613267282339829338903510180606894251700056873567053286214694567429338735367");
    mpz_class q("57306633641169914669451755090303447125850028436783526643107347283714669367683");
    mpz_class g("14777377999967849666226757901157577468444643860798182661856789517423526464749");

    ElGamal_Key_gen key1(p, g);
    const ElGamal_Public_Key pk1 = key1.get_public_key();
    const ElGamal_Private_Key sk1 = key1.get_private_key();

    mpz_class m = 100;
    std::cout << "明文:\t\t" << m << std::endl;

    ElGamal_ciphertext c = ElGamal_Encrypt(m, pk1);
    std::cout << "加密结果r:\t" << c.r << std::endl;
    std::cout << "加密结果s:\t" << c.s << std::endl;

    mpz_class m_decrypt = ElGamal_Decrypt(c, pk1, sk1);
    std::cout << "解密结果:\t" << m_decrypt << std::endl;

    // 推广到一般素数 p
    p = 11;

    ElGamal_Key_gen key2(p);
    const ElGamal_Public_Key pk2 = key2.get_public_key();
    const ElGamal_Private_Key sk2 = key2.get_private_key();

    m = 5;
    std::cout << "明文:\t\t" << m << std::endl;

    c = ElGamal_Encrypt(m, pk2);
    std::cout << "加密结果r:\t" << c.r << std::endl;
    std::cout << "加密结果s:\t" << c.s << std::endl;

    m_decrypt = ElGamal_Decrypt(c, pk2, sk2);
    std::cout << "解密结果:\t" << m_decrypt << std::endl;

    return 0;
}

// 生成随机数
mpz_class generate_random_number(const mpz_class &lowerBound, const mpz_class &upperBound)
{
    mpz_class randomNum;
    gmp_randstate_t state;
    gmp_randinit_default(state);

    mpz_class range = upperBound - lowerBound;
    mpz_urandomm(randomNum.get_mpz_t(), state, range.get_mpz_t());
    randomNum += lowerBound;

    gmp_randclear(state);

    return randomNum;
}

// 计算幂模
mpz_class power_mod(const mpz_class &base, const mpz_class &exp, const mpz_class &mod)
{
    mpz_class result;
    mpz_powm(result.get_mpz_t(), base.get_mpz_t(), exp.get_mpz_t(), mod.get_mpz_t());
    return result;
}

// 扩展欧里几德计算 a^-1 mod b
mpz_class ExEculid(const mpz_class &a, const mpz_class &b)
{
    mpz_class a_copy = a, b_copy = b, x0 = 1, y0 = 0, x1 = 0, y1 = 1, q, r, x, y;

    while (b_copy != 0)
    {
        mpz_divmod(q.get_mpz_t(), r.get_mpz_t(), a_copy.get_mpz_t(), b_copy.get_mpz_t());

        x = x0 - q * x1;
        y = y0 - q * y1;

        a_copy = b_copy;
        b_copy = r;
        x0 = x1;
        y0 = y1;
        x1 = x;
        y1 = y;
    }

    return (x0 + b) % b;
}

// ElGamal加密函数
ElGamal_ciphertext ElGamal_Encrypt(const mpz_class &m, const ElGamal_Public_Key &pk)
{
    ElGamal_ciphertext c;
    mpz_class k = generate_random_number(2, pk.p - 1);

    c.r = power_mod(pk.g, k, pk.p);
    c.s = (power_mod(pk.beta, k, pk.p) * m) % pk.p;

    return c;
}

// ElGamal解密函数
mpz_class ElGamal_Decrypt(const ElGamal_ciphertext &c, const ElGamal_Public_Key &pk, const ElGamal_Private_Key &sk)
{
    return (c.s * ExEculid(power_mod(c.r, sk.alpha, pk.p), pk.p)) % pk.p;
}

// 检查是否是本原根
bool is_primitive_root(const mpz_class &g, const mpz_class &p, const std::vector<mpz_class> &primes)
{
    for (mpz_class q : primes)
    {
        if (power_mod(g, q, p) == 1)
            return false;
    }
    return true;
}

// 生成 Z_p^* 的本原根
mpz_class generate_primitive_root(const mpz_class &p)
{
    std::vector<mpz_class> primes;
    mpz_class phi_p = p - 1, p_square_root;
    mpz_sqrt(p_square_root.get_mpz_t(), phi_p.get_mpz_t());

    // 分解 p-1
    for (mpz_class i = 2; i <= p_square_root; i++)
    {
        if (phi_p % i == 0)
        {
            primes.push_back(i);
            while (phi_p % i == 0)
                phi_p /= i;
        }
    }
    if (phi_p > 1)
        primes.push_back(phi_p);

    // 顺序遍历返回首个本原根
    for (mpz_class g = 2; g < p; g++)
    {
        if (is_primitive_root(g, p, primes))
            return g;
    }

    return 1;
}