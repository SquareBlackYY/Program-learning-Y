#include <iostream>
#include <iomanip>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <gmpxx.h>
#include <cstring>

#define BLOCK_SIZE 32  // 定义拼接字节串每一块的长度


void mpzToBytes(const mpz_class& number, unsigned char* buffer, size_t bufferSize);              // 将 mpz_class 转换为字节数组
void charToBlock(const char* input, unsigned char* output);                                      // 将 char* 转换为 BLOCK_SIZE 字节的 unsigned char 数组
void blockToChar(const unsigned char* input, char* output, size_t bufferSize);                   // 将 unsigned char 数组转换为 char*
void concatenateBlocks(unsigned char* result, std::initializer_list<const unsigned char*> blocks); // 拼接多个块
mpz_class getRandomNumber(const mpz_class &min, const mpz_class &max);                           // 生成指定范围 [min, max] 内的随机整数
mpz_class powMod(const mpz_class &base, const mpz_class &exp, const mpz_class &mod);             // 快速指数幂模运算
void generateRsaKeys(mpz_class &n, mpz_class &e, mpz_class &d);                                  // 生成 RSA 公私钥对
void sha256(const char* input, unsigned char* output);                                           // 使用 OpenSSL EVP 进行 SHA256 哈希计算
mpz_class getInv(mpz_class e, const mpz_class &phi);                                             // 计算模逆元素
mpz_class getPrime(const size_t &bitLength);                                                     // 生成指定 bitLength 位二进制长度的随机素数
void extendedGcd(const mpz_class &f, const mpz_class &d, mpz_class *res);                        // 扩展欧几里德算法
void bytesToMpz(const unsigned char* buffer, mpz_class& number, size_t bufferSize);              // 将字节数组转换为 mpz_class
bool millerRabin(const mpz_class &p);                                                            // Miller-Rabin 素性检测
void guillouQuisquaterProtocol();                                                                // Guillou-Quisquater 协议的主函数

// 签名结构体
struct Signature {
    mpz_class x, y;

    // 重载 << 运算符用于 Signature 结构体的输出
    friend std::ostream& operator<<(std::ostream& os, const Signature& sig) {
        os << "(x: " << sig.x << ", y: " << sig.y << ")";
        return os;
    }
};

// 信任中心类，负责生成 RSA 密钥并分配签名
class TrustAuthority {
public:
    mpz_class n, e, d;        // RSA 密钥对 (n, e) 为公钥, d 为私钥
    TrustAuthority();         // 构造函数生成 RSA 密钥
    mpz_class assignSignature(const mpz_class &jp);  // 为用户分配签名
};

// 用户类，表示用户，同时作为证明者或验证者
class User {
public:
    const char* userId;                         // 用户的身份标识符
    char* ip = nullptr;                         // 用户的 IP 地址
    unsigned char jpHash[SHA256_DIGEST_LENGTH]; // 存储哈希值
    mpz_class jp, sp;                           // 身份哈希值 (jp) 和用户签名 (sp)
    TrustAuthority &ta;                         // 信任中心 (TA) 引用
    bool isProver;                              // 标志用户是证明者还是验证者

    User(const char* identity, TrustAuthority &taRef, bool isProver = true);    // 构造函数生成身份并分配签名
    Signature createSignature(const char *message);                             // 生成签名
    bool verifySignature(User& prover, const char *message, const Signature& sig); // 验证签名
};


int main() {
    guillouQuisquaterProtocol();
    return 0;
}

// Guillou-Quisquater 协议，执行身份验证
void guillouQuisquaterProtocol() {
    TrustAuthority ta;                           // 创建信任中心 (TA)
    User prover("user_identity", ta, true);      // 证明者
    User verifier("verifier_identity", ta, false); // 验证者

    const char* message = "这只是一个测试";

    Signature sig = prover.createSignature(message);

    std::cout << "\t签名为: " << sig << std::endl;

    if (verifier.verifySignature(prover, message, sig)) {
        std::cout << "验签成功" << std::endl;
    } else {
        std::cout << "验签失败" << std::endl;
    }
}

// TrustAuthority 类的构造函数，生成 RSA 密钥对 (n, e, d)
TrustAuthority::TrustAuthority() {
    generateRsaKeys(n, e, d);
}

// 为用户分配签名，计算 SP = (JP^-1)^d mod n
mpz_class TrustAuthority::assignSignature(const mpz_class &jp) {
    return powMod(getInv(jp, n), d, n);
}

// User 类的构造函数，生成身份哈希 JP，并为证明者分配签名
User::User(const char* identity, TrustAuthority &taRef, bool isProver) 
    : userId(identity), ta(taRef), isProver(isProver) {
    sha256(identity, jpHash);
    bytesToMpz(jpHash, jp, SHA256_DIGEST_LENGTH);
    jp %= ta.n;
    if (isProver) {
        sp = ta.assignSignature(jp);  // 分配签名
    }
}

// 生成消息 m 的签名
Signature User::createSignature(const char *message) {
    Signature sig;
    mpz_class r = getRandomNumber(1, ta.n - 1);
    std::cout << "\t生成随机数 r = " << std::hex << r << std::endl;
    sig.x = powMod(r, ta.e, ta.n);

    unsigned char xBytes[BLOCK_SIZE], mBytes[BLOCK_SIZE], input[2 * BLOCK_SIZE], uBytes[BLOCK_SIZE];
    char inputC[2 * BLOCK_SIZE];
    mpzToBytes(sig.x, xBytes, BLOCK_SIZE);
    charToBlock(message, mBytes);
    concatenateBlocks(input, {mBytes, xBytes});
    blockToChar(input, inputC, 2 * BLOCK_SIZE);
    sha256(inputC, uBytes);

    mpz_class u;
    bytesToMpz(uBytes, u, BLOCK_SIZE);
    sig.y = r * powMod(sp, u, ta.n) % ta.n;

    return sig;
}

// 验证签名
bool User::verifySignature(User& prover, const char *message, const Signature& sig) {
    unsigned char xBytes[BLOCK_SIZE], mBytes[BLOCK_SIZE], input[2 * BLOCK_SIZE], uBytes[BLOCK_SIZE];
    char inputC[2 * BLOCK_SIZE];
    mpzToBytes(sig.x, xBytes, BLOCK_SIZE);
    charToBlock(message, mBytes);
    concatenateBlocks(input, {mBytes, xBytes});
    blockToChar(input, inputC, 2 * BLOCK_SIZE);
    sha256(inputC, uBytes);

    mpz_class u;
    bytesToMpz(uBytes, u, BLOCK_SIZE);

    sha256(prover.userId, jpHash);
    bytesToMpz(jpHash, jp, SHA256_DIGEST_LENGTH);
    return (powMod(jp, u, ta.n) * powMod(sig.y, ta.e, ta.n)) % ta.n == sig.x;
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

// 将 char* 转换为 BLOCK_SIZE 字节的 unsigned char 数组
void charToBlock(const char* input, unsigned char* output) {
    int inputLength = std::strlen(input);
    std::memcpy(output, input, inputLength);
    if (inputLength < BLOCK_SIZE)
        std::memset(output + inputLength, ' ', BLOCK_SIZE - inputLength);
}

// 将 unsigned char 数组转换为 char*
void blockToChar(const unsigned char* input, char* output, size_t bufferSize) {
    std::memcpy(output, input, bufferSize);
    int lastIndex = bufferSize - 1;
    while (lastIndex >= 0 && output[lastIndex] == ' ')  // 去除尾部填充
        --lastIndex;
    output[lastIndex + 1] = '\0';  // 设置终止符
}

// 拼接多个块
void concatenateBlocks(unsigned char* result, std::initializer_list<const unsigned char*> blocks) {
    int offset = 0;
    for (auto block : blocks) {
        std::memcpy(result + offset, block, BLOCK_SIZE);
        offset += BLOCK_SIZE;
    }
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
    return rand.get_z_range(range) + min;
}

// 快速指数幂模运算
mpz_class powMod(const mpz_class &base, const mpz_class &exp, const mpz_class &mod) {
    mpz_class result;
    mpz_powm(result.get_mpz_t(), base.get_mpz_t(), exp.get_mpz_t(), mod.get_mpz_t());
    return result;
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

// 计算模逆元素
mpz_class getInv(mpz_class e, const mpz_class &phi) {
    mpz_class res[3];
    if (e > phi) e = e % phi;
    if (e < 0) e = e % phi + phi;
    extendedGcd(phi, e, res);
    if (res[0] != 1) {
        std::cout << "No inverse!" << std::endl;
        exit(0);
    } else {
        return res[2] < 0 ? res[2] + phi : res[2];
    }
}

// RSA 密钥生成函数
void generateRsaKeys(mpz_class &n, mpz_class &e, mpz_class &d) {
    mpz_class p, q, phi;
    p = getPrime(256);
    q = getPrime(256);
    n = p * q;
    phi = (p - 1) * (q - 1);
    do {
        e = getRandomNumber(3, phi - 1);
    } while (gcd(e, phi) != 1);
    d = getInv(e, phi);
}

// 生成指定位长度的随机素数
mpz_class getPrime(const size_t &bitLength) {
    mpz_class beg, end;
    mpz_ui_pow_ui(beg.get_mpz_t(), 2, bitLength - 1);
    mpz_ui_pow_ui(end.get_mpz_t(), 2, bitLength);
    mpz_class p;
    bool isPrime;
    do {
        isPrime = true;
        p = getRandomNumber(beg, end - 1);
        if (p != 2 && (p & 1) == 0) p++;
        bool flag = false;
        for (size_t i = 0; i < 6; ++i) {
            flag = millerRabin(p);
            if (!flag) {
                isPrime = false;
                break;
            }
        }
    } while (!isPrime);
    return p;
}

// 扩展欧几里德算法
void extendedGcd(const mpz_class &f, const mpz_class &d, mpz_class *res) {
    mpz_class x[3] = {1, 0, f};
    mpz_class y[3] = {0, 1, d};
    mpz_class t[3];
    mpz_class q;
    while (true) {
        if (y[2] == 0) {
            res[0] = x[2];
            return;
        }
        if (y[2] == 1) {
            res[0] = y[2], res[1] = y[0], res[2] = y[1];
            return;
        }
        q = x[2] / y[2];
        t[0] = x[0] - q * y[0], t[1] = x[1] - q * y[1], t[2] = x[2] - q * y[2];
        x[0] = y[0], x[1] = y[1], x[2] = y[2];
        y[0] = t[0], y[1] = t[1], y[2] = t[2];
    }
}

// 将字节数组转换为 mpz_class 类型
void bytesToMpz(const unsigned char* buffer, mpz_class& number, size_t bufferSize) {
    number = 0;
    mpz_import(number.get_mpz_t(), bufferSize, 1, sizeof(buffer[0]), 0, 0, buffer);
}

// Miller-Rabin 素性检测
bool millerRabin(const mpz_class &p) {
    size_t k = 0;
    mpz_class t = p - 1;
    while ((t & 1) == 0) {
        t = t >> 1;
        k++;
    }
    mpz_class a = getRandomNumber(2, p - 1);
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
