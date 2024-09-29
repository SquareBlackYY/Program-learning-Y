#include <iostream>
#include <gmpxx.h>
#include <openssl/evp.h>
using namespace std;

#define INF Point(NULL, NULL)

mpz_class powm(const mpz_class &, const mpz_class &, const mpz_class &);
mpz_class getRandomNumber(const mpz_class &, const mpz_class &, const mpz_class &coprime = 0);
mpz_class getInv(const mpz_class &, const mpz_class &);

class Point
{
public:
    mpz_class x;
    mpz_class y;
    mpz_class p;
    mpz_class a;

    Point() {}
    Point(const mpz_class &x, const mpz_class &y) : x(x), y(y) {}
    Point(const mpz_class &x, const mpz_class &y, const mpz_class &p, const mpz_class &a) : x(x), y(y), p(p), a(a) {}

    friend ostream &operator<<(ostream &os, const Point &p)
    {
        os << "(" << p.x << ", " << p.y << ")";
        return os;
    }

    Point operator-() const
    {
        return Point(x, -y, p, a);
    }

    bool operator==(const Point &another) const
    {
        if (p != 0)
            return ((x % p + p) % p == (another.x % p + p) % p && (y % p + p) % p == (another.y % p + p) % p);
        else
            return (x == another.x && y == another.y);
    }

    bool operator!=(const Point &another) const
    {
        if (p != 0)
            return ((x % p + p) % p != (another.x % p + p) % p || (y % p + p) % p != (another.y % p + p) % p);
        else
            return (x != another.x || y != another.y);
    }

    Point operator+(const Point &another) const
    {
        mpz_class lambda;
        if (*this == INF)
            return another;
        else if (another == INF)
            return *this;
        else if (*this == -another)
            return INF;
        else if (*this == another)
            lambda = (3 * x * x + a) * getInv(2 * y, p) % p;
        else
            lambda = (another.y - y) * getInv(another.x - x, p) % p;

        Point sum;
        sum.x = ((lambda * lambda - x - another.x) % p + p) % p;
        sum.y = ((lambda * (x - sum.x) - y) % p + p) % p;
        sum.p = p;
        sum.a = a;
        return sum;
    }

    void operator+=(const Point &another)
    {
        *this = (*this + another);
    }

    friend Point operator*(mpz_class multiple, const Point &p)
    {
        Point temp = p, result = INF;
        while (multiple != 0)
        {
            if ((multiple & 1) == 1)
                result += temp;
            temp += temp;
            multiple >>= 1;
        }
        return result;
    }
};

class ECDSAPrivateKey
{
public:
    mpz_class d;

    Point G;
    mpz_class n;

    ECDSAPrivateKey(const mpz_class &d, const Point &G, const mpz_class &n) : d(d), G(G), n(n) {}

    friend ostream &operator<<(ostream &os, const ECDSAPrivateKey &sk)
    {
        os << "ECDSA私钥:" << endl;
        os << "d : " << sk.d << endl;
        return os;
    }
};

class ECDSAPublicKey
{
public:
    Point Q;

    Point G;
    mpz_class n;

    ECDSAPublicKey(const Point &Q, const Point &G, const mpz_class &n) : Q(Q), G(G), n(n) {}

    friend ostream &operator<<(ostream &os, const ECDSAPublicKey &pk)
    {
        os << "ECDSA公钥:" << endl;
        os << "Q : " << pk.Q << endl;

        return os;
    }
};

class ECDSAParameter
{
public:
    mpz_class p;
    mpz_class a;
    mpz_class b;
    Point G;
    mpz_class n;

    ECDSAParameter(const mpz_class &p, const mpz_class &a, const mpz_class &b, const Point &G) : p(p), a(a), b(b), G(G)
    {
        this->G.p = p;
        this->G.a = a;

        Point point = INF;
        do
        {
            point += this->G;
            n++;
        } while (point != INF);
    }

    Point setPoint(const mpz_class &x, const mpz_class &y)
    {
        return Point(x, y, p, a);
    }

    ECDSAPrivateKey getPrivateKey()
    {
        return ECDSAPrivateKey(getRandomNumber(1, n), G, n);
    }

    ECDSAPublicKey getPublicKey(const ECDSAPrivateKey &sk)
    {
        return ECDSAPublicKey(sk.d * G, G, n);
    }
};

class ECDSASignature
{
public:
    mpz_class r;
    mpz_class s;

    ECDSASignature(const mpz_class r, const mpz_class s) : r(r), s(s) {}

    friend ostream &operator<<(ostream &os, const ECDSASignature &sc)
    {
        os << "ECDSA签名:" << endl;
        os << "r : " << sc.r << endl;
        os << "s : " << sc.s << endl;
        return os;
    }
};

mpz_class stringToMpz(const string &);
string mpzToString(const mpz_class &);
mpz_class sha256(const mpz_class &);

ECDSASignature ECDSASign(const ECDSAPrivateKey &, const string &);
bool ECDSAVerify(const ECDSAPublicKey &, const ECDSASignature &, const string &);

int main()
{
    ECDSAParameter ecdsa(23, 1, 4, Point(0, 2));

    ECDSAPrivateKey sk = ecdsa.getPrivateKey();
    ECDSAPublicKey pk = ecdsa.getPublicKey(sk);

    cout << pk << endl << sk << endl;
    
    string m = "aaa";
    cout << "明文:" << m << endl
         << endl;

    // 签名
    ECDSASignature sig = ECDSASign(sk, m);
    cout << sig << endl;

    // 验签
    cout << "验签:" << (ECDSAVerify(pk, sig, m) ? "签名合法" : "签名非法") << endl;

    return 0;
}

ECDSASignature ECDSASign(const ECDSAPrivateKey &sk, const string &m_str)
{
    const mpz_class e = sha256(stringToMpz(m_str));

    mpz_class k, r, s;
    Point point;

    do
    {
        do
        {
            k = getRandomNumber(1, sk.n);
            point = k * sk.G;
            r = point.x % sk.n;
        } while (r == 0);
        s = getInv(k, sk.n) * (e + sk.d * r) % sk.n;
    } while (s == 0);

    return ECDSASignature(r, s);
}

bool ECDSAVerify(const ECDSAPublicKey &pk, const ECDSASignature &sig, const string &m_str)
{
    if (sig.r < 1 || sig.r >= pk.n || sig.s < 1 || sig.s >= pk.n)
        return false;

    const mpz_class e = sha256(stringToMpz(m_str));

    const mpz_class w = getInv(sig.s, pk.n);

    const mpz_class u1 = e * w % pk.n;
    const mpz_class u2 = sig.r * w % pk.n;

    const Point X = u1 * pk.G + u2 * pk.Q;

    if (X == INF)
        return false;
    else if (X.x % pk.n == sig.r)
        return true;
    return false;
}

// 模幂函数
mpz_class powm(const mpz_class &base, const mpz_class &power, const mpz_class &mod)
{
    mpz_class result;
    mpz_powm(result.get_mpz_t(), base.get_mpz_t(), power.get_mpz_t(), mod.get_mpz_t());
    return result;
}

// 生成随机数（前闭后开）要求互素
mpz_class getRandomNumber(const mpz_class &lowerBound, const mpz_class &upperBound, const mpz_class &coprime)
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
    } while (coprime != 0 && getInv(result, coprime) == 0);

    return result;
}

// 扩展欧里几得算法求模逆 a ^ -1 mod b
mpz_class getInv(const mpz_class &a, const mpz_class &b)
{
    mpz_class x1 = 1, x2 = 0, x3 = b, y1 = 0, y2 = 1, y3 = (a % b + b) % b;
    mpz_class q, t1, t2, t3;

    while (y3 > 1)
    {
        q = x3 / y3;

        t1 = x1 - q * y1;
        t2 = x2 - q * y2;
        t3 = x3 % y3;

        x1 = y1;
        x2 = y2;
        x3 = y3;

        y1 = t1;
        y2 = t2;
        y3 = t3;
    }

    return y3 == 1 ? (y2 % b + b) % b : y3;
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

// 哈希函数SHA256
mpz_class sha256(const mpz_class &input)
{
    const string inputStr = input.get_str();

    // 定义哈希输出缓冲区和长度
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int length = 0;

    // 创建 EVP 上下文
    EVP_MD_CTX *context = EVP_MD_CTX_new();
    if (context == nullptr)
        return 0;

    // 使用 EVP 接口进行哈希计算
    if (EVP_DigestInit_ex(context, EVP_sha256(), nullptr) &&
        EVP_DigestUpdate(context, inputStr.c_str(), inputStr.size()) &&
        EVP_DigestFinal_ex(context, hash, &length))
    {
        // 将哈希结果转换为 mpz_class 对象
        mpz_class result;
        mpz_import(result.get_mpz_t(), length, 1, sizeof(hash[0]), 0, 0, hash);

        // 释放 EVP 上下文
        EVP_MD_CTX_free(context);
        return result;
    }

    // 哈希计算失败
    EVP_MD_CTX_free(context);
    return 0;
}