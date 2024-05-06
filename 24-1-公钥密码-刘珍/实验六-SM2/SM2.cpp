#include <iostream>
#include <gmpxx.h>
#include <cstdint>
#include <vector>

mpz_class generate_random_number(const mpz_class &, const mpz_class &);
struct byteString int_to_bytes(const mpz_class &);
mpz_class bytes_to_int(const byteString &);
mpz_class ExEculid(const mpz_class &, const mpz_class &);
Point ECC_add(const Elliptic_Curve &, const Point &, const Point &);
Point ECC_multiple_add(const Elliptic_Curve &, const Point &, mpz_class);

// 点坐标
struct Point
{
    mpz_class x;
    mpz_class y;

    Point() : x(0), y(0) {}
    Point(const mpz_class &x_val, const mpz_class &y_val) : x(x_val), y(y_val) {}
    // 字符串形式初始化大整数
    Point(const std::string &x_val, const std::string &y_val) : x(x_val), y(y_val) {}
};

// 椭圆曲线
struct Elliptic_Curve
{
    mpz_class p;    // 有限域F(p)中元素的数目
    mpz_class a, b; // 椭圆曲线系数
    Point G;        // 基点
    mpz_class n;    // 基点G的阶
    mpz_class h;    // 余因子

    Elliptic_Curve(const mpz_class &p_val, const mpz_class &a_val, const mpz_class &b_val, const Point &G_point, const mpz_class &n_val, const mpz_class &h_val)
        : p(p_val), a(a_val), b(b_val), G(G_point), n(n_val), h(h_val) {}
    // 字符串形式初始化大整数
    Elliptic_Curve(const std::string &p_val, const std::string &a_val, const std::string &b_val, const Point &G_point, const std::string &n_val, const mpz_class &h_val)
        : p(p_val), a(a_val), b(b_val), G(G_point), n(n_val), h(h_val) {}
};

// 字节串
class byteString
{
public:
    size_t size;
    uint8_t *string;

    ~byteString()
    {
        delete[] string;
    }
    void PrintString()
    {
        std::cout << "字节串: ";
        for (size_t i = 0; i < size; i++)
            std::cout << std::hex << static_cast<int>(string[i]);
        std::cout << std::dec << std::endl;
    }
};

int main()
{
    Elliptic_Curve E("0xfffffffe ffffffff ffffffff ffffffff ffffffff 00000000 ffffffff ffffffff",
                     "0xfffffffe ffffffff ffffffff ffffffff ffffffff 00000000 ffffffff fffffffc",
                     "0x28e9fa9e 9d9f5e34 4d5a9e4b cf6509a7 f39789f5 15ab8f92 ddbcbd41 4d940e93",
                     Point("0x32c4ae2c 1f198119 5f990446 6a39c994 8fe30bbf f2660be1 715a4589 334c74c7", "0xbc3736a2 f4f6779c 59bdcee3 6b692153 d0a9877c c62a4740 02df32e5 2139f0a0"),
                     "0xfffffffe ffffffff ffffffff ffffffff 7203df6b 21c6052b 53bbf409 39d54123",
                     1);

    mpz_class d = generate_random_number(1, E.n); // 用户B的私钥
    Point P = ECC_multiple_add(E, E.G, d);        // 用户B的公钥

    mpz_class num("258");

    // 大整数转字节串
    byteString bytestring = int_to_bytes(num);

    bytestring.PrintString();

    // 字节串转大整数
    mpz_class result = bytes_to_int(bytestring);

    std::cout << std::dec << "大整数: " << result << std::endl;

    return 0;
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

// 椭圆曲线群的加法运算
Point ECC_add(const Elliptic_Curve &E, const Point &p, const Point &q)
{
    mpz_class lambda;
    if (p.x == 0 && p.y == 0)
        return q;
    else if (q.x == 0 && q.y == 0)
        return p;
    else if (p.x == q.x && (p.y % E.p + E.p) % E.p == ((-q.y) % E.p + E.p) % E.p)
        return Point((mpz_class)0, 0);
    else if (p.x == q.x && p.y == q.y)
        lambda = ((3 * p.x * p.x + E.a) * ExEculid(2 * p.y, E.p)) % E.p;
    else
        lambda = ((q.y - p.y) * ExEculid(q.x - p.x, E.p)) % E.p;

    Point sum;
    sum.x = ((lambda * lambda - p.x - q.x) % E.p + E.p) % E.p;
    sum.y = ((lambda * (p.x - sum.x) - p.y) % E.p + E.p) % E.p;
    return sum;
}

// 倍点运算
Point ECC_multiple_add(const Elliptic_Curve &E, const Point &p, mpz_class multiple)
{
    Point temp = p, result((mpz_class)0, 0);
    while (multiple)
    {
        if ((multiple & 1) == 1)
            result = ECC_add(E, result, temp);
        temp = ECC_add(E, temp, temp);
        multiple >>= 1;
    }
    return result;
}

// 整数转字节串
struct byteString int_to_bytes(const mpz_class &Num)
{
    byteString bytestring;
    bytestring.size = (mpz_sizeinbase(Num.get_mpz_t(), 2) + 7) / 8;
    bytestring.string = new uint8_t[bytestring.size];
    mpz_export(bytestring.string, nullptr, 1, 1, 1, 0, Num.get_mpz_t());
    return bytestring;
}

// 字节串转整数
mpz_class bytes_to_int(const byteString &bytestring)
{
    mpz_class Num;
    mpz_import(Num.get_mpz_t(), bytestring.size, 1, 1, 1, 0, bytestring.string);
    return Num;
}