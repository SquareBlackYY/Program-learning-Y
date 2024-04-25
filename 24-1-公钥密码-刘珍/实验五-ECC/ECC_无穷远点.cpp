#include <iostream>
#include <gmpxx.h>
#include <vector>

struct Coordinate
{
    mpz_class x;
    mpz_class y;

    Coordinate() {}
    Coordinate(const mpz_class x, const mpz_class y) : x(x), y(y) {}
};

struct Elliptic_Curve
{
    mpz_class p;
    mpz_class a;
    mpz_class b;

    Elliptic_Curve(const mpz_class p, const mpz_class a, const mpz_class b) : p(p), a{a}, b(b) {}
};

Coordinate ECC_add(const Elliptic_Curve &, const Coordinate &, const Coordinate &);
Coordinate ECC_multiple_add(const Elliptic_Curve &, const Coordinate &, int);
mpz_class power_mod(const mpz_class &, const mpz_class &, const mpz_class &);
mpz_class ExEculid(const mpz_class &, const mpz_class &);
mpz_class generate_random_number(const mpz_class &, const mpz_class &);

class ECDH
{
private:
    Elliptic_Curve E; // 椭圆曲线
    Coordinate G;     // 生成元
    mpz_class n;      // G的阶
    mpz_class rn;     // 随机数

public:
    ECDH(const Elliptic_Curve &E, const Coordinate &G, const mpz_class &n) : E(E), G(G), n(n)
    {
        rn = generate_random_number(2, n);
    };

    Coordinate generate_public_key()
    {
        return {(rn * G.x) % E.p, (rn * G.y) % E.p};
    }

    Coordinate generate_share_key(const Coordinate &PK)
    {
        return {(rn * PK.x) % E.p, (rn * PK.y) % E.p};
    }
};

int main()
{
    std::cout << "实例: a = 1, b = 4, p = 23, n = 29, G = (0, 2)" << std::endl;
    const Elliptic_Curve E(23, 1, 4);
    const Coordinate p(0, 2);
    const int multiple = 29;
    std::cout << "结果: (" << ECC_multiple_add(E, p, multiple).x << ", " << ECC_multiple_add(E, p, multiple).y << ")" << std::endl;

    std::cout << "ECDH算法实现:" << std::endl;
    ECDH A({211, 0, -4}, {2, 2}, 241), B({211, 0, -4}, {2, 2}, 241);
    Coordinate PK_A = A.generate_public_key(), PK_B = B.generate_public_key();
    std::cout << "A向B发出的公钥: (" << PK_A.x <<  ", " << PK_A.y << ")" << std::endl;
    std::cout << "B向A发出的公钥: (" << PK_B.x <<  ", " << PK_B.y << ")" << std::endl;
    std::cout << "A计算出的共享密钥: (" << A.generate_share_key(PK_B).x <<  ", " << A.generate_share_key(PK_B).y << ")" << std::endl;
    std::cout << "B计算出的共享密钥: (" << B.generate_share_key(PK_A).x <<  ", " << B.generate_share_key(PK_A).y << ")" << std::endl;

    return 0;
}

// 椭圆曲线群的加法运算
Coordinate ECC_add(const Elliptic_Curve &E, const Coordinate &p, const Coordinate &q)
{
    mpz_class lambda;
    if (p.x == NULL && p.y == NULL)
        return q;
    else if (q.x == NULL && q.y == NULL)
        return p;
    else if (p.x == q.x && (p.y % E.p + E.p) % E.p == ((-q.y) % E.p + E.p) % E.p)
        return Coordinate(NULL, NULL);
    else if (p.x == q.x && p.y == q.y)
        lambda = ((3 * p.x * p.x + E.a) * ExEculid(2 * p.y, E.p)) % E.p;
    else
        lambda = ((q.y - p.y) * ExEculid(q.x - p.x, E.p)) % E.p;

    Coordinate sum;
    sum.x = ((lambda * lambda - p.x - q.x) % E.p + E.p) % E.p;
    sum.y = ((lambda * (p.x - sum.x) - p.y) % E.p + E.p) % E.p;
    return sum;
}

// 倍点运算
Coordinate ECC_multiple_add(const Elliptic_Curve &E, const Coordinate &p, int multiple)
{
    Coordinate temp = p, result(0, 0);
    while (multiple)
    {
        if ((multiple & 1) == 1)
            result = ECC_add(E, result, temp);
        temp = ECC_add(E, temp, temp);
        multiple >>= 1;
    }
    return result;
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