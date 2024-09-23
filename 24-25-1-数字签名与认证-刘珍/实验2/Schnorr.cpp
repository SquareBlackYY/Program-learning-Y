#include <iostream>
#include <gmpxx.h>
#include <openssl/evp.h>
using namespace std;

mpz_class powm(const mpz_class &, const mpz_class &, const mpz_class &);
mpz_class generateRandomNumber(const mpz_class &, const mpz_class &);

class SchnorrPrivateKey
{
public:
    mpz_class p;
    mpz_class q;
    mpz_class g;
    mpz_class x;

    SchnorrPrivateKey(const mpz_class &p, const mpz_class &q, const mpz_class &g, const mpz_class &x) : p(p), q(q), g(g), x(x) {}

    friend ostream &operator<<(ostream &os, const SchnorrPrivateKey &sk)
    {
        os << "Schnorr私钥:" << endl;
        os << "x : " << sk.x << endl;
        return os;
    }
};

class SchnorrPublicKey
{
public:
    mpz_class p;
    mpz_class q;
    mpz_class g;
    mpz_class y;

    SchnorrPublicKey(const mpz_class &p, const mpz_class &q, const mpz_class &g, const mpz_class &y) : p(p), q(q), g(g), y(y) {}

    friend ostream &operator<<(ostream &os, const SchnorrPublicKey &pk)
    {
        os << "Schnorr公钥:" << endl;
        os << "p : " << pk.p << endl;
        os << "q : " << pk.q << endl;
        os << "g : " << pk.g << endl;
        os << "y : " << pk.y << endl;
        return os;
    }
};

class SchnorrParameter
{
private:
    mpz_class p;
    mpz_class q;
    mpz_class g;

public:
    SchnorrParameter(const mpz_class &p, const mpz_class &q, const mpz_class &g) : p(p), q(q), g(g) {}

    SchnorrPrivateKey generatePrivateKey()
    {
        const mpz_class x = generateRandomNumber(2, q);
        return SchnorrPrivateKey(p, q, g, x);
    }
    SchnorrPrivateKey generatePrivateKey(const mpz_class &x)
    {
        return SchnorrPrivateKey(p, q, g, x);
    }

    SchnorrPublicKey generatePublicKey(const SchnorrPrivateKey &sk)
    {
        const mpz_class y = powm(g, sk.x, p);
        return SchnorrPublicKey(p, q, g, y);
    }
};

class SchnorrSignature
{
public:
    mpz_class r;
    mpz_class s;

    SchnorrSignature(const mpz_class r, const mpz_class s) : r(r), s(s) {}

    friend ostream &operator<<(ostream &os, const SchnorrSignature &sc)
    {
        os << "Schnorr密文:" << endl;
        os << "r : " << sc.r << endl;
        os << "s : " << sc.s << endl;
        return os;
    }
};

mpz_class sha256(const mpz_class &);
mpz_class modInverse(const mpz_class &, const mpz_class &);

mpz_class stringToMpz(const string &);
string mpzToString(const mpz_class &);

SchnorrSignature schnorrSign(const SchnorrPrivateKey &, const string &);
bool schnorrVerify(const SchnorrPublicKey &, const SchnorrSignature &, const string &);

int main()
{
    SchnorrParameter kp(mpz_class("0xb9c0faef108e0da9bc6a7fd87b9b837a19c2fa3b2daa3ef276fd87fcb7fe690f"), mpz_class("0x169f3edc9665f26b65fc6e805e3c997160a388a376e9266cfdbb512e107"), mpz_class("0x5119a79a9849f5c98659566b890077dfa71a7cb22f92e9089d3462b3b2bc16fa"));

    SchnorrPrivateKey sk = kp.generatePrivateKey(mpz_class("0x7C5D9F8B4A2F30D2E5AFA59F3C7B9A18E0DDAF23"));
    SchnorrPublicKey pk = kp.generatePublicKey(sk);

    string m = "This is a test message for Schnorr signature";

    cout << pk << endl << sk << endl;

    cout << "明文:" << m << endl << endl;

    // 签名
    SchnorrSignature c = schnorrSign(sk, m);
    cout << c << endl;

    // 验签
    cout << "验签:" << (schnorrVerify(pk, c, m) ? "签名合法" : "签名非法") << endl;

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

// 扩展欧里几得算法求模逆 a^-1 mod b
mpz_class modInverse(const mpz_class &a, const mpz_class &b)
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

    return (x0 + b) % b;
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

// Schnorr签名函数
SchnorrSignature schnorrSign(const SchnorrPrivateKey &sk, const string &m_str)
{
    const mpz_class m = stringToMpz(m_str);

    const mpz_class k = generateRandomNumber(1, sk.q);
    const mpz_class w = powm(sk.g, k, sk.p);

    const mpz_class r = sha256((w << mpz_sizeinbase(sk.p.get_mpz_t(), 2)) + m);
    const mpz_class s = (k + sk.x * r) % sk.q;

    return SchnorrSignature(r, s);
}

// Schnorr验签函数
bool schnorrVerify(const SchnorrPublicKey &pk, const SchnorrSignature &sc, const string & m_str)
{
    const mpz_class m = stringToMpz(m_str);

    const mpz_class w_prime = (powm(pk.g, sc.s, pk.p) * powm(modInverse(pk.y, pk.p), sc.r, pk.p)) % pk.p;
    const mpz_class r_prime = sha256((w_prime << mpz_sizeinbase(pk.p.get_mpz_t(), 2)) + m);

    return sc.r == r_prime;
}