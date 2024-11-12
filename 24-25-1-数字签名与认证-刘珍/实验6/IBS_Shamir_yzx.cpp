#include <iostream>
#include <gmpxx.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/rand.h>

#define BLOCK_SIZE 32  // 定义拼接字节串每一块的长度


mpz_class gcd(mpz_class a, mpz_class b);                                            // 计算两个数的最大公约数
void gcdExt(const mpz_class &f, const mpz_class &d, mpz_class *res);                // 扩展欧几里得算法，用于计算 ax + by = gcd(a,b)
mpz_class getInv(mpz_class e, const mpz_class &phi);                                // 求逆元，计算 e 在模 phi 下的乘法逆元
mpz_class getRandomNumber(const mpz_class &min, const mpz_class &max);              // 生成指定范围 [min, max] 内的随机整数
mpz_class powMod(const mpz_class &base, const mpz_class &exp, const mpz_class &mod);// 快速指数幂模运算，计算 (base^exp) mod mod
bool millerRabin(const mpz_class &p);                                               // Miller-Rabin 素性测试，判断 p 是否为素数
mpz_class getPrime(const size_t &bitLength);                                        // 生成指定二进制位长度的随机素数
void mpzToBytes(const mpz_class& number, unsigned char* buffer, size_t bufferSize); // 将 mpz_class 转换为字节数组
void bytesToMpz(const unsigned char* buffer, mpz_class& number, size_t bufferSize); // 将字节数组转换为 mpz_class
void charToBlock(const char* input, unsigned char* output);                         // 将 char* 转换为 BLOCK_SIZE 字节的 unsigned char 数组
void blockToChar(const unsigned char* input, char* output, size_t bufferSize);      // 将 unsigned char 数组转换为 char*
void concatenateBlocks(unsigned char* result, std::initializer_list<const unsigned char*> blocks);  // 拼接多个块，将多个 BLOCK_SIZE 大小的块拼接到 result 中
void sha256(const char* input, unsigned char* output);                              // 使用 OpenSSL EVP 进行 SHA256 哈希计算
void shamirSignatureId();                                                           // 函数 shamirSignatureId 的声明

// 签名结构体
struct Signature {
    mpz_class s, t;
    friend std::ostream& operator<<(std::ostream& os, const Signature& sig);
};

// KGC 类
class KGC {
public:
    mpz_class p, q, N, e, d;
    void (*hash)(const char*, unsigned char*);
    KGC(const size_t &bitLength = 512, const mpz_class &pVal = 0, const mpz_class &qVal = 0, const mpz_class &eRange = 0);
};

// User 类
class User {
public:
    mpz_class g, N, e;
    const char* id;
    void (*hash)(const char*, unsigned char*);
    User(const char* userId, const KGC kgc);
    Signature createSignature(const char* message);
    bool verifySignature(User& prover, const char* message, const Signature& sig);
};


int main() {
    shamirSignatureId();
    return 0;
}


// Shamir 数字签名方案的主函数
void shamirSignatureId() {
    KGC kgc(512);
    User prover("Alice", kgc);
    User verifier("Bob", kgc);

    const char* message = "这只是一个测试";

    Signature sig = prover.createSignature(message);

    std::cout << "\t签名为: " << sig << std::endl;

    if (verifier.verifySignature(prover, message, sig)) {
        std::cout << "验签成功" << std::endl;
    } else {
        std::cout << "验签失败" << std::endl;
    }
}

// 计算两个数的最大公约数
mpz_class gcd(mpz_class a, mpz_class b) {
    mpz_class tmp = 0;
    while (b != 0) {
        tmp = a % b;
        a = b;
        b = tmp;
    }
    return a;
}

// 扩展欧几里得算法，用于计算 ax + by = gcd(a,b)
void gcdExt(const mpz_class &f, const mpz_class &d, mpz_class *res) {
    mpz_class x[3] = {1, 0, f};
    mpz_class y[3] = {0, 1, d};
    mpz_class t[3];
    mpz_class Q;
    while (true) {
        if (y[2] == 0) {
            res[0] = x[2];
            return;
        }
        if (y[2] == 1) {
            res[0] = y[2];
            res[1] = y[0];
            res[2] = y[1];
            return;
        }
        Q = x[2] / y[2];
        t[0] = x[0] - Q * y[0];
        t[1] = x[1] - Q * y[1];
        t[2] = x[2] - Q * y[2];
        x[0] = y[0];
        x[1] = y[1];
        x[2] = y[2];
        y[0] = t[0];
        y[1] = t[1];
        y[2] = t[2];
    }
}

// 求逆元，计算 e 在模 phi 下的乘法逆元
mpz_class getInv(mpz_class e, const mpz_class &phi) {
    mpz_class res[3];
    if (e > phi)
        e = e % phi;
    if (e < 0)
        e = e % phi + phi;
    gcdExt(phi, e, res);
    if (res[0] != 1)
        throw std::invalid_argument("No inverse!");
    else
        return res[2] < 0 ? res[2] + phi : res[2];
}

// 生成指定范围 [min, max] 内的随机整数
mpz_class getRandomNumber(const mpz_class &min, const mpz_class &max) {
    static gmp_randclass rand(gmp_randinit_default);
    static bool seedInitialized = false;
    if (!seedInitialized) {
        rand.seed(time(NULL));
        seedInitialized = true;
    }

    mpz_class range = max - min + 1;
    mpz_class randomNum = rand.get_z_range(range) + min;
    return randomNum;
}

// 快速指数幂模运算，计算 (base^exp) mod mod
mpz_class powMod(const mpz_class &base, const mpz_class &exp, const mpz_class &mod) {
    mpz_class result;
    mpz_powm(result.get_mpz_t(), base.get_mpz_t(), exp.get_mpz_t(), mod.get_mpz_t());
    return result;
}

// Miller-Rabin 素性测试，判断 p 是否为素数
bool millerRabin(const mpz_class &p) {
    size_t k = 0;
    mpz_class t = p - 1;
    while ((t & 1) == 0) {  // 判断是否为偶数
        t = t >> 1;          // 除二操作
        k++;
    }
    mpz_class a = getRandomNumber(2, p - 1);  // 在 [2, p - 1] 之间选一个底数
    mpz_class b = powMod(a, t, p);
    if (b == 1)
        return true;
    for (size_t j = 0; j <= k; j++) {
        if ((b + 1) % p == 0)
            return true;
        b = b * b % p;
    }
    return false;
}

// 生成指定二进制位长度的随机素数
mpz_class getPrime(const size_t &bitLength) {
    mpz_class beg, end;
    mpz_ui_pow_ui(beg.get_mpz_t(), 2, bitLength - 1);
    mpz_ui_pow_ui(end.get_mpz_t(), 2, bitLength);
    mpz_class p;
    bool isPrime;
    do {
        isPrime = true;
        p = getRandomNumber(beg, end - 1);  // 生成 [2 ^ (bitLength - 1), 2 ^ bitLength - 1] 之间的随机数
        if (p != 2 && (p & 1) == 0)
            p++;
        bool flag = false;
        for (size_t i = 0; i < 6; ++i) {  // 进行6轮 Miller-Rabin 素性测试
            flag = millerRabin(p);
            if (!flag) {
                isPrime = false;
                break;
            }
        }
    } while (!isPrime);
    return p;
}

// 将 mpz_class 转换为字节数组
void mpzToBytes(const mpz_class& number, unsigned char* buffer, size_t bufferSize) {
    std::memset(buffer, 0, bufferSize);  // 清空缓冲区
    size_t count;
    mpz_export(buffer, &count, 1, sizeof(buffer[0]), 0, 0, number.get_mpz_t());
    if (count < bufferSize) {
        size_t padding = bufferSize - count;
        std::memmove(buffer + padding, buffer, count);  // 将数据向右移动
        std::memset(buffer, 0, padding);  // 左侧填充0
    }
}

// 将字节数组转换为 mpz_class
void bytesToMpz(const unsigned char* buffer, mpz_class& number, size_t bufferSize) {
    number = 0;  // 清空 mpz_class 对象的内容
    mpz_import(number.get_mpz_t(), bufferSize, 1, sizeof(buffer[0]), 0, 0, buffer);
}

// 将 char* 转换为 BLOCK_SIZE 字节的 unsigned char 数组
void charToBlock(const char* input, unsigned char* output) {
    int inputLength = std::strlen(input);
    std::memcpy(output, input, inputLength);
    if (inputLength < BLOCK_SIZE)
        std::memset(output + inputLength, ' ', BLOCK_SIZE - inputLength);  // 用空格填充不足部分
}

// 将 unsigned char 数组转换为 char*
void blockToChar(const unsigned char* input, char* output, size_t bufferSize) {
    std::memcpy(output, input, bufferSize);
    int lastIndex = bufferSize - 1;
    while (lastIndex >= 0 && output[lastIndex] == ' ')  // 去除尾部填充
        --lastIndex;
    output[lastIndex + 1] = '\0';  // 设置终止符
}

// 拼接多个块，将多个 BLOCK_SIZE 大小的块拼接到 result 中
void concatenateBlocks(unsigned char* result, std::initializer_list<const unsigned char*> blocks) {
    int offset = 0;
    for (auto block : blocks) {
        std::memcpy(result + offset, block, BLOCK_SIZE);
        offset += BLOCK_SIZE;
    }
}

// 使用 OpenSSL EVP 进行 SHA256 哈希计算
void sha256(const char* input, unsigned char* output) {
    EVP_MD_CTX* context = EVP_MD_CTX_new();
    const EVP_MD* md = EVP_sha256();
    EVP_DigestInit_ex(context, md, nullptr);
    EVP_DigestUpdate(context, input, strlen(input));
    EVP_DigestFinal_ex(context, output, nullptr);
    EVP_MD_CTX_free(context);
}

// 重载 Signature 结构体的 << 运算符，用于输出签名
std::ostream& operator<<(std::ostream& os, const Signature& sig) {
    os << "(x: " << sig.s << ", y: " << sig.t << ")";
    return os;
}

// KGC 类的构造函数，实现密钥生成中心的初始化
KGC::KGC(const size_t &bitLength, const mpz_class &pVal, const mpz_class &qVal, const mpz_class &eRange)
    : p(pVal), q(qVal), hash(sha256)
{
    if (pVal == 0 && qVal == 0) {  // 若用户没有给定 p, q，则生成随机素数
        do {
            p = getPrime(bitLength);
            q = getPrime(bitLength);
        } while (p == q);
    }

    N = p * q;
    mpz_class phi = (p - 1) * (q - 1);

    if (eRange == 0) {  // 若用户没有给定 e，则生成一个随机整数 e
        do {
            e = getRandomNumber(3, phi - 1);
        } while (gcd(e, phi) != 1);
    } else {
        do {
            e = getRandomNumber(3, eRange);
        } while (gcd(e, phi) != 1);
    }
    d = getInv(e, phi);
}

// User 类的构造函数，实现用户的初始化
User::User(const char* userId, const KGC kgc)
    : id(userId), N(kgc.N), e(kgc.e), hash(kgc.hash) {
    unsigned char idBytes[BLOCK_SIZE];
    mpz_class idDigit;
    charToBlock(id, idBytes);
    bytesToMpz(idBytes, idDigit, BLOCK_SIZE);
    g = powMod(idDigit, kgc.d, N);
}

// User 类的成员函数，创建签名
Signature User::createSignature(const char* message) {
    Signature sig;
    mpz_class r;
    do {
        r = getRandomNumber(2, N - 1);
    } while (gcd(r, N) != 1);

    sig.t = powMod(r, e, N);

    unsigned char tBytes[BLOCK_SIZE], mBytes[BLOCK_SIZE], input[2 * BLOCK_SIZE], cBytes[SHA256_DIGEST_LENGTH];
    char inputC[2 * BLOCK_SIZE];
    mpzToBytes(sig.t, tBytes, BLOCK_SIZE);
    charToBlock(message, mBytes);
    concatenateBlocks(input, {tBytes, mBytes});
    blockToChar(input, inputC, 2 * BLOCK_SIZE);
    hash(inputC, cBytes);

    mpz_class c;
    bytesToMpz(cBytes, c, SHA256_DIGEST_LENGTH);
    sig.s = g * powMod(r, c, N) % N;

    return sig;
}

// User 类的成员函数，验证签名
bool User::verifySignature(User& prover, const char* message, const Signature& sig) {
    unsigned char idBytes[BLOCK_SIZE];
    mpz_class idDigit;
    charToBlock(prover.id, idBytes);
    bytesToMpz(idBytes, idDigit, BLOCK_SIZE);

    unsigned char tBytes[BLOCK_SIZE], mBytes[BLOCK_SIZE], input[2 * BLOCK_SIZE], cBytes[SHA256_DIGEST_LENGTH];
    char inputC[2 * BLOCK_SIZE];
    mpzToBytes(sig.t, tBytes, BLOCK_SIZE);
    charToBlock(message, mBytes);
    concatenateBlocks(input, {tBytes, mBytes});
    blockToChar(input, inputC, 2 * BLOCK_SIZE);
    hash(inputC, cBytes);

    mpz_class c;
    bytesToMpz(cBytes, c, SHA256_DIGEST_LENGTH);

    return powMod(sig.s, e, N) == idDigit * powMod(sig.t, c, N) % N;
}
