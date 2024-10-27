#include <iostream>
#include <iomanip>
#include <gmpxx.h>
#include <openssl/evp.h>
#include <openssl/sha.h>

#define BLOCK_SIZE 32  // 定义拼接字节串每一块的长度

void mpzToBytes(const mpz_class& number, unsigned char* buffer, size_t bufferSize);                 // 将 mpz_class 转换为字节数组
void bytesToMpz(const unsigned char* buffer, mpz_class& number, size_t bufferSize);                 // 将字节数组转换为 mpz_class
void charToBlock(const char* input, unsigned char* output);                                         // 将 char* 转换为 BLOCK_SIZE 字节的 unsigned char 数组
void blockToChar(const unsigned char* input, char* output, size_t bufferSize);                      // 将 unsigned char 数组转换为 char*
void concatenateBlocks(unsigned char* result, std::initializer_list<const unsigned char*> blocks);  // 拼接多个块
void sha256(const char* input, unsigned char* output);                                              // 使用 OpenSSL EVP 进行 SHA256 哈希计算
mpz_class getRandomNumber(const mpz_class& min, const mpz_class& max);                              // 生成指定范围 [min, max] 内的随机整数
mpz_class powMod(const mpz_class& base, const mpz_class& exp, const mpz_class& mod);                // 快速指数幂模运算
bool millerRabinTest(const mpz_class& p);                                                           // Miller-Rabin 素性检测
mpz_class generatePrime(const size_t& bitLength);                                                   // 生成指定位长度的随机素数
void fiatShamirSig();                                                                               // Fiat-Shamir 识别协议 -> 签名协议

// 签名结构体
struct Signature {
    mpz_class x, y;

    // 重载 << 运算符作为 Signature 结构体的友元函数
    friend std::ostream& operator<<(std::ostream& os, const Signature& sig) {
        os << "(x: " << sig.x << ", y: " << sig.y << ")";
        return os;
    }
};

// 用户类声明
class User {
private:
    mpz_class p, q, n, s, v;  // 用户的私钥和公钥信息

public:
    User(const size_t& bitLength = 256, const mpz_class& pVal = 0, const mpz_class& qVal = 0, const mpz_class& sVal = 0);

    Signature createSignature(const char* message);                                  // 生成签名
    bool verifySignature(User& prover, const char* message, const Signature& sig);  // 验证签名

    // 获取私钥 p, q, s
    mpz_class getP() const;
    mpz_class getQ() const;
    mpz_class getS() const;
};

// 主函数部分
int main() {
    fiatShamirSig();
    return 0;
}

// Fiat-Shamir 识别协议 -> 签名协议
void fiatShamirSig() {
    User prover(256);  // 证明者
    User verifier(256, prover.getP(), prover.getQ(), prover.getS());  // 验证者

    const char* message = "这只是一个测试";

    Signature sig = prover.createSignature(message);

    std::cout << "\t签名为: " << sig << std::endl;

    if (verifier.verifySignature(prover, message, sig)) {
        std::cout << "验签成功" << std::endl;
    } else {
        std::cout << "验签失败" << std::endl;
    }
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

// 拼接多个块
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

// 生成指定范围 [min, max] 内的随机整数
mpz_class getRandomNumber(const mpz_class& min, const mpz_class& max) {
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

// 快速指数幂模运算
mpz_class powMod(const mpz_class& base, const mpz_class& exp, const mpz_class& mod) {
    mpz_class result;
    mpz_powm(result.get_mpz_t(), base.get_mpz_t(), exp.get_mpz_t(), mod.get_mpz_t());
    return result;
}

// Miller-Rabin 素性检测
bool millerRabinTest(const mpz_class& p) {
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

// 生成指定位长度的随机素数
mpz_class generatePrime(const size_t& bitLength) {
    mpz_class beg, end;
    mpz_ui_pow_ui(beg.get_mpz_t(), 2, bitLength - 1);
    mpz_ui_pow_ui(end.get_mpz_t(), 2, bitLength);
    mpz_class p;
    bool isPrime;
    do {
        isPrime = true;
        p = getRandomNumber(beg, end - 1);
        if (p != 2 && (p & 1) == 0)
            p++;
        for (size_t i = 0; i < 6; ++i) {
            if (!millerRabinTest(p)) {
                isPrime = false;
                break;
            }
        }
    } while (!isPrime);
    return p;
}

// 用户类构造函数
User::User(const size_t& bitLength, const mpz_class& pVal, const mpz_class& qVal, const mpz_class& sVal)
    : p(pVal), q(qVal), s(sVal) {
    if (pVal == 0) p = generatePrime(bitLength);
    if (qVal == 0) q = generatePrime(bitLength);
    n = p * q;
    if (sVal == 0) s = getRandomNumber(1, n - 1);
    v = powMod(s, 2, n);
}

// 生成消息 m 的签名
Signature User::createSignature(const char* message) {
    Signature sig;
    mpz_class r = getRandomNumber(2, n - 1);
    std::cout << "\t生成随机数 r = " << std::hex << r << std::endl;
    sig.x = powMod(r, 2, n);

    unsigned char xBytes[BLOCK_SIZE], mBytes[BLOCK_SIZE], input[2 * BLOCK_SIZE], cBytes[BLOCK_SIZE];
    char inputC[2 * BLOCK_SIZE];
    mpzToBytes(sig.x, xBytes, BLOCK_SIZE);
    charToBlock(message, mBytes);
    concatenateBlocks(input, {mBytes, xBytes});
    blockToChar(input, inputC, 2 * BLOCK_SIZE);
    sha256(inputC, cBytes);

    mpz_class c;
    bytesToMpz(cBytes, c, BLOCK_SIZE);
    sig.y = r * powMod(s, c, n) % n;
    
    return sig;
}

// 验证消息 m 的签名
bool User::verifySignature(User& prover, const char* message, const Signature& sig) {
    unsigned char xBytes[BLOCK_SIZE], mBytes[BLOCK_SIZE], input[2 * BLOCK_SIZE], cBytes[BLOCK_SIZE];
    char inputC[2 * BLOCK_SIZE];
    mpzToBytes(sig.x, xBytes, BLOCK_SIZE);
    charToBlock(message, mBytes);
    concatenateBlocks(input, {mBytes, xBytes});
    blockToChar(input, inputC, 2 * BLOCK_SIZE);
    sha256(inputC, cBytes);

    mpz_class c;
    bytesToMpz(cBytes, c, BLOCK_SIZE);

    return powMod(sig.y, 2, n) == sig.x * powMod(prover.v, c, n) % n;
}

// 获取 p
mpz_class User::getP() const { return p; }

// 获取 q
mpz_class User::getQ() const { return q; }

// 获取 s
mpz_class User::getS() const { return s; }
