#include <iostream>
#include <gmpxx.h>
using namespace std;

mpz_class powm(const mpz_class &, const mpz_class &, const mpz_class &);
mpz_class generateRandomNumber(const mpz_class &, const mpz_class &);

class ElGamalPrivateKey
{
public:
    mpz_class p;
    mpz_class g;
    mpz_class x;

    ElGamalPrivateKey(const mpz_class &p, const mpz_class &g, const mpz_class &x) : p(p), g(g), x(x) {}

    friend ostream &operator<<(ostream &os, const ElGamalPrivateKey &sk)
    {
        os << "ElGamal私钥:" << endl;
        os << "x : " << sk.x << endl;
        return os;
    }
};

class ElGamalPublicKey
{
public:
    mpz_class p;
    mpz_class g;
    mpz_class y;

    ElGamalPublicKey(const mpz_class &p, const mpz_class &g, const mpz_class &y) : p(p), g(g), y(y) {}

    friend ostream &operator<<(ostream &os, const ElGamalPublicKey &pk)
    {
        os << "ElGamal公钥:" << endl;
        os << "p : " << pk.p << endl;
        os << "g : " << pk.g << endl;
        os << "y : " << pk.y << endl;
        return os;
    }
};

class ElGamalParameter
{
private:
    mpz_class p;
    mpz_class g;

public:
    ElGamalParameter(const mpz_class &p, const mpz_class &g) : p(p), g(g) {}

    ElGamalPrivateKey generatePrivateKey()
    {
        const mpz_class x = generateRandomNumber(1, p - 1);
        return ElGamalPrivateKey(p, g, x);
    }
    ElGamalPrivateKey generatePrivateKey(const mpz_class &x)
    {
        return ElGamalPrivateKey(p, g, x);
    }

    ElGamalPublicKey generatePublicKey(const ElGamalPrivateKey &sk)
    {
        const mpz_class y = powm(g, sk.x, p);
        return ElGamalPublicKey(p, g, y);
    }
};

class ElGamalSignature
{
public:
    mpz_class r;
    mpz_class s;

    ElGamalSignature(const mpz_class r, const mpz_class s) : r(r), s(s) {}

    friend ostream &operator<<(ostream &os, const ElGamalSignature &sc)
    {
        os << "ElGamal签名:" << endl;
        os << "r : " << sc.r << endl;
        os << "s : " << sc.s << endl;
        return os;
    }
};

mpz_class ExEculid(const mpz_class &, const mpz_class &);
bool isCoprime(const mpz_class &, const mpz_class &);
mpz_class getInv(const mpz_class &, const mpz_class &);

mpz_class stringToMpz(const string &);
string mpzToString(const mpz_class &);

ElGamalSignature ElGamalSign(const ElGamalPrivateKey &, const string &);
bool ElGamalVerify(const ElGamalPublicKey &, const ElGamalSignature &, const string &);

int main()
{
    ElGamalParameter kp(mpz_class("0xaa5ae5a2ff388b78174378cfdea0f7363893a63c68227df45ed8be2cde31241f"), mpz_class("0x5"));

    ElGamalPrivateKey sk = kp.generatePrivateKey(mpz_class("0x4A5B6C7D8E9F0A1B2C3D4E5F6A7B8C9"));
    ElGamalPublicKey pk = kp.generatePublicKey(sk);

    string m = "This is a test message for ElGamal signature";

    cout << pk << endl
         << sk << endl;

    cout << "明文:" << m << endl
         << endl;

    // 签名
    ElGamalSignature sig = ElGamalSign(sk, m);
    cout << sig << endl;

    // 验签
    cout << "验签:" << (ElGamalVerify(pk, sig, m) ? "签名合法" : "签名非法") << endl;

    return 0;
}

// 模幂函数
mpz_class powm(const mpz_class &base, const mpz_class &power, const mpz_class &mod)
{
    mpz_class result;
    mpz_powm(result.get_mpz_t(), base.get_mpz_t(), power.get_mpz_t(), mod.get_mpz_t());
    return result;
}

// 生成随机数（前闭后开）
mpz_class generateRandomNumber(const mpz_class &lowerBound, const mpz_class &upperBound)
{
    static gmp_randclass rand(gmp_randinit_mt);
    static bool ifSeedInitialized = false;
    if (!ifSeedInitialized)
    {
        rand.seed(time(0));
        ifSeedInitialized = true;
    }

    return lowerBound + rand.get_z_range(upperBound - lowerBound);
}

// 生成随机数（前闭后开）要求互素
mpz_class generateRandomNumber(const mpz_class &lowerBound, const mpz_class &upperBound, const mpz_class &coprime)
{
    static gmp_randclass rand(gmp_randinit_mt);
    static bool ifSeedInitialized = false;
    if (!ifSeedInitialized)
    {
        rand.seed(time(0));
        ifSeedInitialized = true;
    }

    mpz_class result;
    do
    {
        result = lowerBound + rand.get_z_range(upperBound - lowerBound);
    } while (!isCoprime(result, coprime));

    return result;
}

// 扩展欧里几得算法
mpz_class ExEculid(const mpz_class &a, const mpz_class &b)
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

    return x0;
}

// 互素判断
bool isCoprime(const mpz_class &a, const mpz_class &b)
{
    return (a > b ? ExEculid(a, b) : ExEculid(b, a)) != 1;
}

// 求逆 a ^ -1 mod b
mpz_class getInv(const mpz_class &a, const mpz_class &b)
{
    return ExEculid(b, ((a % b) + b) % b);
}

// 字符串转整数
mpz_class stringToMpz(const string &str)
{
    mpz_class result = 0;
    for (size_t i = 0; i < str.length(); ++i)
        result = (result << 8) | str[i]; // 每个字节向左移动 8 位，然后按位或存储字符
    return result;
}

// 整数转字符串
string mpzToString(const mpz_class &num)
{
    mpz_class n = num;
    vector<unsigned char> bytes;

    while (n > 0)
    {
        unsigned char byte = static_cast<unsigned char>(n.get_ui()); // 获取最低字节
        bytes.push_back(byte);
        n >>= 8; // 右移 8 位处理下一个字节
    }

    // 反转字节顺序，构造出 UTF-8 字符串
    reverse(bytes.begin(), bytes.end());
    return string(bytes.begin(), bytes.end());
}

// ElGamal签名函数
ElGamalSignature ElGamalSign(const ElGamalPrivateKey &sk, const string &m_str)
{
    const mpz_class m = stringToMpz(m_str);

    const mpz_class k = generateRandomNumber(1, sk.p - 1, sk.p - 1);

    const mpz_class r = powm(sk.g, k, sk.p);
    const mpz_class s = (getInv(k, sk.p - 1) * (m - sk.x * r)) % (sk.p - 1);

    return ElGamalSignature(r, s);
}

// ElGamal验签函数
bool ElGamalVerify(const ElGamalPublicKey &pk, const ElGamalSignature &sig, const string &m_str)
{
    const mpz_class m = stringToMpz(m_str);

    return (powm(pk.y, sig.r, pk.p) * powm(sig.r, sig.s, pk.p)) % pk.p == powm(pk.g, m, pk.p);
}