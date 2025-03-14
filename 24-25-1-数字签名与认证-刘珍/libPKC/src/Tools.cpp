//
// Created by 张笑宇 on 2024/9/29.
//

#include "Tools.h"

using namespace std;

mpz_class powm(const mpz_class &base, const mpz_class &power, const mpz_class &mod) {
    mpz_class result;
    mpz_powm(result.get_mpz_t(), base.get_mpz_t(), power.get_mpz_t(), mod.get_mpz_t());
    return result;
}

mpz_class getRandNum(const mpz_class &lowerBound, const mpz_class &upperBound) {
    static gmp_randclass rand(gmp_randinit_mt);
    static bool ifSeedInitialized = false;
    if (!ifSeedInitialized) {
        rand.seed(time(nullptr));
        ifSeedInitialized = true;
    }
    return lowerBound + rand.get_z_range(upperBound - lowerBound);
}

mpz_class getRandNumWithCoprime(const mpz_class &lowerBound, const mpz_class &upperBound, const mpz_class &coprime) {
    static gmp_randclass rand(gmp_randinit_mt);
    static bool ifSeedInitialized = false;
    if (!ifSeedInitialized) {
        rand.seed(time(nullptr));
        ifSeedInitialized = true;
    }

    mpz_class result;
    do {
        result = lowerBound + rand.get_z_range(upperBound - lowerBound);
    } while (coprime != 0 && !isCoprime(result, coprime));

    return result;
}

vector<mpz_class> exEuclid(const mpz_class &a, const mpz_class &b) {
    vector<mpz_class> x{1, 0, a}, y{0, 1, b};
    while (y[2] != 0) {
        mpz_class q = x[2] / y[2];
        const vector<mpz_class> t = {x[0] - q * y[0], x[1] - q * y[1], x[2] % y[2]};
        x = y;
        y = t;
    }
    return x;
}

mpz_class gcd(const mpz_class &a, const mpz_class &b) {
    return exEuclid(a, b)[2];  // 直接返回 gcd
}

mpz_class getInv(const mpz_class &a, const mpz_class &b) {
    const vector<mpz_class> result = exEuclid((a % b + b) % b, b);

    // 如果 gcd(a, b) == 1，返回逆元；否则返回 0，表示逆元不存在
    if (result[2] == 1) {
        return (result[0] % b + b) % b;  // 确保结果为正
    }

    return 0;  // 无法求逆
}

bool isCoprime(const mpz_class &a, const mpz_class &b) {
    return exEuclid(a, b)[2] == 1;
}

mpz_class stringToMpz(const string &str) {
    mpz_class result = 0;
    for (size_t i = 0; i < str.length(); ++i)
        result = (result << 8) | str[i]; // 每个字节向左移动 8 位，然后按位或存储字符
    return result;
}

string mpzToString(const mpz_class &num) {
    mpz_class n = num;
    vector<unsigned char> bytes;

    while (n > 0) {
        auto byte = static_cast<unsigned char>(n.get_ui()); // 获取最低字节
        bytes.push_back(byte);
        n >>= 8; // 右移 8 位处理下一个字节
    }

    // 反转字节顺序，构造出 UTF-8 字符串
    reverse(bytes.begin(), bytes.end());
    return {bytes.begin(), bytes.end()};
}

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

string sha256(const string &input) {
    // 定义哈希输出缓冲区和长度
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int length = 0;

    // 创建 EVP 上下文
    EVP_MD_CTX *context = EVP_MD_CTX_new();
    if (context == nullptr) {
        return "";
    }

    // 使用 EVP 接口进行哈希计算
    if (EVP_DigestInit_ex(context, EVP_sha256(), nullptr) &&
        EVP_DigestUpdate(context, input.c_str(), input.size()) &&
        EVP_DigestFinal_ex(context, hash, &length)) {

        // 将哈希结果转换为十六进制字符串
        std::string hashStr;
        for (unsigned int i = 0; i < length; ++i) {
            char buffer[3];  // 两位十六进制数 + 空字符
            snprintf(buffer, sizeof(buffer), "%02x", hash[i]);
            hashStr += buffer;
        }

        // 释放 EVP 上下文
        EVP_MD_CTX_free(context);
        return hashStr;
        }

    // 哈希计算失败
    EVP_MD_CTX_free(context);
    return "";
}