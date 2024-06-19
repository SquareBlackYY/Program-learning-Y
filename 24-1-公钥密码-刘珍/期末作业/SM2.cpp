#include <iostream>
#include <gmpxx.h>
#include <cstdint>
#include <iomanip>
#include <vector>
#include <openssl/sha.h>

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

// SM2密文
struct SM2_ciphertext
{
    std::vector<bool> C1, C2, C3;
    SM2_ciphertext(const std::vector<bool> &c1, const std::vector<bool> &c2, const std::vector<bool> &c3) : C1(c1), C2(c2), C3(c3) {}
};

mpz_class generate_random_number(const mpz_class &, const mpz_class &);
mpz_class ExEculid(const mpz_class &, const mpz_class &);
Point ECC_add(const Elliptic_Curve &, const Point &, const Point &);
Point ECC_multiple_add(const Elliptic_Curve &, const Point &, mpz_class);
mpz_class power_mod(const mpz_class &, const mpz_class &, const mpz_class &);
mpz_class sqrt_mod(const mpz_class &, const mpz_class &);
void Lucas_sequence_Gen(const mpz_class &, const mpz_class &, const mpz_class &, const mpz_class &, mpz_class &, mpz_class &);

std::vector<uint8_t> int_to_bytes(const mpz_class &);
mpz_class bytes_to_int(const std::vector<uint8_t> &);
void int_to_bits(const mpz_class &, std::vector<bool> &);
void bits_to_int(const std::vector<bool> &, mpz_class &);
void bytes_to_bits(const std::vector<uint8_t> &, std::vector<bool> &);
void bits_to_bytes(const std::vector<bool> &, std::vector<uint8_t> &);
void Point_to_bytes(const Elliptic_Curve &, const Point &, std::vector<uint8_t> &, const int);
void bytes_to_Point(const Elliptic_Curve &, const std::vector<uint8_t> &, Point &);
void PrintByteString(const std::vector<uint8_t> &);
void KDF(std::vector<uint8_t>, const size_t &, std::vector<bool> &);
SM2_ciphertext SM2_Encrypt(const Elliptic_Curve &, const Point &, const std::vector<bool> &, const size_t &);
std::vector<bool> SM2_Decrypt(const Elliptic_Curve &, const mpz_class &, const SM2_ciphertext &, const size_t &);

int main()
{
    // 密钥生成
    Elliptic_Curve E("0xfffffffe ffffffff ffffffff ffffffff ffffffff 00000000 ffffffff ffffffff",
                     "0xfffffffe ffffffff ffffffff ffffffff ffffffff 00000000 ffffffff fffffffc",
                     "0x28e9fa9e 9d9f5e34 4d5a9e4b cf6509a7 f39789f5 15ab8f92 ddbcbd41 4d940e93",
                     Point("0x32c4ae2c 1f198119 5f990446 6a39c994 8fe30bbf f2660be1 715a4589 334c74c7", "0xbc3736a2 f4f6779c 59bdcee3 6b692153 d0a9877c c62a4740 02df32e5 2139f0a0"),
                     "0xfffffffe ffffffff ffffffff ffffffff 7203df6b 21c6052b 53bbf409 39d54123",
                     1);
    mpz_class d_B = generate_random_number(1, E.n); // 用户B的私钥
    Point P_B = ECC_multiple_add(E, E.G, d_B);      // 用户B的公钥

    mpz_class m_int("0x0123456789abcdef");
    std::vector<uint8_t> m_bytes = int_to_bytes(m_int);
    std::cout << "明文：\t";
    PrintByteString(m_bytes);

    size_t klen = mpz_sizeinbase(m_int.get_mpz_t(), 2); // 计算比特位数
    std::vector<bool> M(klen);
    int_to_bits(m_int, M);

    SM2_ciphertext C = SM2_Encrypt(E, P_B, M, klen);

    std::cout << "加密结果：" << std::endl
              << "C1:" << std::endl;
    for (bool bit : C.C1)
        std::cout << bit;
    std::cout << std::endl
              << "C2:" << std::endl;
    for (bool bit : C.C2)
        std::cout << bit;
    std::cout << std::endl
              << "C3:" << std::endl;
    for (bool bit : C.C3)
        std::cout << bit;
    std::cout << std::endl;

    std::vector<bool> M_decrypt = SM2_Decrypt(E, d_B, C, klen);
    mpz_class m_decrypt = 0;
    bits_to_int(M_decrypt, m_decrypt);
    std::vector<uint8_t> m_decrypt_bytes = int_to_bytes(m_decrypt);

    std::cout << "解密结果：\t";
    PrintByteString(m_decrypt_bytes);

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
        return Point();
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
    Point temp = p, result;
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

// 模素数平方根求解
mpz_class sqrt_mod(const mpz_class &p, const mpz_class &g)
{
    if (p % 4 == 3)
    {
        mpz_class u = (p - 3) / 4;
        mpz_class y = power_mod(g, u + 1, p);
        mpz_class z = power_mod(y, 2, p);
        if (z == g)
            return y;
        else
            exit(0);
    }
    else if (p % 8 == 5)
    {
        mpz_class u = (p - 5) / 8;
        mpz_class z = power_mod(g, 2 * u + 1, p);
        if (z % p == 1)
            return power_mod(g, u + 1, p);
        else if (z % p == p - 1)
            return 2 * g * power_mod(4 * g, u, p) % p;
        else
            exit(0);
    }
    else if (p % 8 == 1)
    {
        mpz_class u = (p - 1) / 8;
        mpz_class Y = g;

        while (true)
        {
            mpz_class X = generate_random_number(1, p);
            mpz_class U, V;
            Lucas_sequence_Gen(p, X, Y, 4 * u + 1, U, V);
            if (V * V % p == 4 * Y % p)
                return V / 2 % p;
            if (U % p != 1 && U % p != p - 1)
                exit(0);
        }
    }

    return -1;
}

// Lucas 序列生成
void Lucas_sequence_Gen(const mpz_class &p, const mpz_class &X, const mpz_class &Y, const mpz_class &k, mpz_class &U, mpz_class &V)
{
    mpz_class delta = X * X - 4 * Y;
    size_t r = mpz_sizeinbase(k.get_mpz_t(), 2); // 计算比特位数
    U = 1, V = X;
    for (size_t i = 0; i < r; ++i)
    {
        mpz_class tmp = U;
        U = U * V % p;
        V = (V * V + delta * tmp * tmp) / 2 % p;

        mpz_class ki = k >> (r - i - 1) & 1;
        if (mpz_get_ui(ki.get_mpz_t()) == 1)
        {
            tmp = U;
            U = (X * U + V) / 2 % p;
            V = (X * V + delta * tmp) / 2 % p;
        }
    }
}

// 整数转字节串
std::vector<uint8_t> int_to_bytes(const mpz_class &Num)
{
    size_t size;
    unsigned char *bytes = reinterpret_cast<unsigned char *>(mpz_export(nullptr, &size, 1, sizeof(uint8_t), 1, 0, Num.get_mpz_t()));
    std::vector<uint8_t> bytestring(bytes, bytes + size);
    free(bytes);
    return bytestring;
}

// 字节串转整数
mpz_class bytes_to_int(const std::vector<uint8_t> &bytestring)
{
    mpz_class Num;
    mpz_import(Num.get_mpz_t(), bytestring.size(), 1, sizeof(uint8_t), 1, 0, bytestring.data());
    return Num;
}

// 整数转比特串
void int_to_bits(const mpz_class &m, std::vector<bool> &M)
{
    size_t r = mpz_sizeinbase(m.get_mpz_t(), 2);
    for (size_t i = 0; i < r; ++i)
        M[i] = mpz_tstbit(m.get_mpz_t(), r - i - 1);
}

// 比特串转整数
void bits_to_int(const std::vector<bool> &M, mpz_class &m)
{
    for (int i = 0; i < M.size(); ++i)
        m = m << 1 | M[i];
}

// 将字节串转换为比特串
void bytes_to_bits(const std::vector<uint8_t> &bytes, std::vector<bool> &bits)
{
    bits.resize(bytes.size() * 8);
    for (size_t i = 0; i < bytes.size(); ++i)
        for (size_t j = 0; j < 8; ++j)
            bits[8 * i + j] = bytes[i] >> (7 - j) & 1;
}

// 将比特串转换为字节串
void bits_to_bytes(const std::vector<bool> &bits, std::vector<uint8_t> &bytes)
{
    int bytes_len = bits.size() / 8;
    bytes.resize(bytes_len);
    for (size_t i = 0; i < bytes_len; ++i)
        for (size_t j = 0; j < 8; ++j)
            bytes[i] |= bits[8 * i + j] << (7 - j);
}

// 点根据不同形式转字节串
void Point_to_bytes(const Elliptic_Curve &E, const Point &P, std::vector<uint8_t> &bytes, const int model)
{
    size_t bytesLen = mpz_sizeinbase(E.p.get_mpz_t(), 256) + 1; // 计算需要的字节数
    bytes.resize(bytesLen);
    // 为保证总长为 l + 1，PC 与 X之间用 0 填充
    size_t numBytes = mpz_sizeinbase(P.x.get_mpz_t(), 256);
    for (size_t i = bytesLen - numBytes; i < bytesLen; ++i)
    {
        mpz_class byte = (P.x >> ((bytesLen - i - 1) * 8)) & 0xff; // 取出每个字节
        bytes[i] |= mpz_get_ui(byte.get_mpz_t());
    }

    if (model == 0)
    {                                                    // 压缩表示
        bool y = mpz_tstbit(P.y.get_mpz_t(), 0) ? 1 : 0; // 计算 P.y 最右边一个比特
        uint8_t PC = !y ? 0x02 : 0x03;
        bytes[0] = PC;
    }
    else if (model == 1)
    { // 未压缩表示
        uint8_t PC = 0x04;
        bytes[0] = PC;

        bytesLen = mpz_sizeinbase(E.p.get_mpz_t(), 256) * 2 + 1;
        bytes.resize(bytesLen);
        numBytes = mpz_sizeinbase(P.y.get_mpz_t(), 256);
        for (size_t i = bytesLen - numBytes; i < bytesLen; ++i)
        {
            mpz_class byte = (P.y >> ((bytesLen - i - 1) * 8)) & 0xff;
            bytes[i] |= mpz_get_ui(byte.get_mpz_t());
        }
    }
    else if (model == 2)
    { // 混合表示
        bool y = mpz_tstbit(P.y.get_mpz_t(), 0) ? 1 : 0;
        uint8_t PC = !y ? 0x06 : 0x07;
        bytes[0] = PC;

        bytesLen = mpz_sizeinbase(E.p.get_mpz_t(), 256) * 2 + 1;
        bytes.resize(bytesLen);
        numBytes = mpz_sizeinbase(P.y.get_mpz_t(), 256);
        for (size_t i = bytesLen - numBytes; i < bytesLen; ++i)
        {
            mpz_class byte = (P.y >> ((bytesLen - i - 1) * 8)) & 0xff;
            bytes[i] |= mpz_get_ui(byte.get_mpz_t());
        }
    }
    else
        exit(0);
}

// 字节串根据不同形式转点
void bytes_to_Point(const Elliptic_Curve &E, const std::vector<uint8_t> &bytes, Point &P)
{
    size_t bytesLen = mpz_sizeinbase(E.p.get_mpz_t(), 256) + 1; // 计算需要的字节数
    size_t i;
    for (i = 1; i < bytesLen && bytes[i] == 0; ++i)
        ;

    for (; i < bytesLen; ++i) // 恢复横坐标
        P.x = (P.x << 8) | bytes[i];

    if (bytes[0] == 0x02 || bytes[0] == 0x03)
    { // 压缩表示
        bool y = bytes[0] == 0x02 ? 0 : 1;
        mpz_class alpha = (power_mod(P.x, 3, E.p) + E.a * P.x + E.b) % E.p;
        mpz_class beta = sqrt_mod(E.p, alpha);
        P.y = mpz_tstbit(beta.get_mpz_t(), 0) == y ? beta : E.p - beta;
    }
    else if (bytes[0] == 0x04)
    { // 未压缩表示
        size_t Totalbytes = mpz_sizeinbase(E.p.get_mpz_t(), 256) * 2 + 1;
        for (i = bytesLen; i < Totalbytes && bytes[i] == 0; ++i)
            ;

        for (; i < Totalbytes; ++i)
            P.y = (P.y << 8) | bytes[i];
    }
    else if (bytes[0] == 0x06 || bytes[0] == 0x07)
    { // 混合表示
        srand((unsigned)time(NULL));
        bool model = rand() % 2;
        if (model)
        {
            size_t Totalbytes = mpz_sizeinbase(E.p.get_mpz_t(), 256) * 2 + 1;
            for (i = bytesLen; i < Totalbytes && bytes[i] == 0; ++i)
                ;

            for (; i < Totalbytes; ++i)
                P.y = (P.y << 8) | bytes[i];
        }
        else
        {
            bool y = bytes[0] == 0x06 ? 0 : 1;
            mpz_class alpha = (power_mod(P.x, 3, E.p) + E.a * P.x + E.b) % E.p;
            mpz_class beta = sqrt_mod(E.p, alpha);
            P.y = mpz_tstbit(beta.get_mpz_t(), 0) == y ? beta : E.p - beta;
        }
    }

    if (power_mod(P.y, 2, E.p) != (power_mod(P.x, 3, E.p) + E.a * P.x + E.b) % E.p)
        exit(0);
}

// 打印字节串
void PrintByteString(const std::vector<uint8_t> &bytestring)
{
    for (uint8_t byte : bytestring)
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    std::cout << std::dec << std::endl;
}

// 密钥派生函数
void KDF(std::vector<uint8_t> bytes, const size_t &klen, std::vector<bool> &t)
{
    size_t size = (klen + 512 - 1) / 512;
    uint8_t Ct = 0x1;
    bytes.push_back(Ct);
    uint8_t *message = bytes.data();
    size_t length = bytes.size();

    for (size_t i = 1; i < size; i++)
    {
        uint8_t digest[SHA512_DIGEST_LENGTH];
        SHA512(message, length, digest);
        std::vector<uint8_t> data_bytes(digest, digest + SHA512_DIGEST_LENGTH);
        std::vector<bool> data_bits;
        bytes_to_bits(data_bytes, data_bits);
        std::copy(data_bits.begin(), data_bits.end(), t.begin() + (i - 1) * 512);
        message[length - 1] = i + 1;
    }
    uint8_t digest[SHA512_DIGEST_LENGTH];
    SHA512(message, length, digest);
    std::vector<uint8_t> data_bytes(digest, digest + SHA512_DIGEST_LENGTH);
    std::vector<bool> data_bits;
    bytes_to_bits(data_bytes, data_bits);
    std::copy(data_bits.begin(), data_bits.begin() + klen - (size - 1) * 512, t.begin() + (size - 1) * 512);
}

// SM2加密
SM2_ciphertext SM2_Encrypt(const Elliptic_Curve &E, const Point &P, const std::vector<bool> &M, const size_t &klen)
{
    while (true)
    {
        std::vector<uint8_t> C1_bytes;
        std::vector<bool> C1_bits, C2_bits(klen), C3_bits, t(klen);

        // 1.选择随机数k, k∈[1, n - 1]
        mpz_class k = generate_random_number(1, E.n);
        // 2.计算C1 = k * G = (x1, y1)
        Point C1_point = ECC_multiple_add(E, E.G, k);

        srand((unsigned)time(NULL));
        int model = rand() % 3; // 随机选择转换模式
        Point_to_bytes(E, C1_point, C1_bytes, model);
        bytes_to_bits(C1_bytes, C1_bits);

        // 3.计算S = h * P_B, 若S为无穷远点则退出
        Point S = ECC_multiple_add(E, P, E.h);
        if (S.x == 0 && S.y == 0)
            exit(0);

        // 4.计算k * P_B = (x2, y2)
        Point Q = ECC_multiple_add(E, P, k);

        std::vector<uint8_t> x2_bytes = int_to_bytes(Q.x);
        std::vector<uint8_t> y2_bytes = int_to_bytes(Q.y);
        std::vector<uint8_t> Q_bytes(x2_bytes.size() + y2_bytes.size());

        std::copy(x2_bytes.begin(), x2_bytes.end(), Q_bytes.begin());
        std::copy(y2_bytes.begin(), y2_bytes.end(), Q_bytes.begin() + x2_bytes.size());
        KDF(Q_bytes, klen, t);
        if (std::all_of(t.begin(), t.end(), [](bool value)
                        { return !value; })) // t 为全 0 比特串
            continue;
        std::transform(M.begin(), M.end(), t.begin(), C2_bits.begin(), [](bool a, bool b)
                       { return a ^ b; });

        std::vector<uint8_t> M_bytes;
        bits_to_bytes(M, M_bytes);
        std::vector<uint8_t> message(x2_bytes.size() + M_bytes.size() + y2_bytes.size());

        std::vector<uint8_t>::iterator iter = message.begin();
        iter = std::copy(x2_bytes.begin(), x2_bytes.end(), iter);
        iter = std::copy(M_bytes.begin(), M_bytes.end(), iter);
        std::copy(y2_bytes.begin(), y2_bytes.end(), iter);

        uint8_t *hash_val = message.data();
        size_t length = message.size();
        uint8_t digest[SHA512_DIGEST_LENGTH];
        SHA512(hash_val, length, digest);
        std::vector<uint8_t> data_bytes(digest, digest + SHA512_DIGEST_LENGTH);
        bytes_to_bits(data_bytes, C3_bits);

        return {C1_bits, C2_bits, C3_bits};
    }
}

// SM2解密
std::vector<bool> SM2_Decrypt(const Elliptic_Curve &E, const mpz_class &d, const SM2_ciphertext &C, const size_t &klen)
{
    std::vector<uint8_t> C1_bytes;
    std::vector<bool> t(klen), M(klen), u;
    Point C1_point;

    bits_to_bytes(C.C1, C1_bytes);
    bytes_to_Point(E, C1_bytes, C1_point);
    if (power_mod(C1_point.y, 2, E.p) != (power_mod(C1_point.x, 3, E.p) + E.a * C1_point.x + E.b) % E.p)
        exit(0);

    Point S = ECC_multiple_add(E, C1_point, E.h);
    if (S.x == 0 && S.y == 0)
        exit(0);

    Point Q = ECC_multiple_add(E, C1_point, d);
    std::vector<uint8_t> x2_bytes = int_to_bytes(Q.x);
    std::vector<uint8_t> y2_bytes = int_to_bytes(Q.y);
    std::vector<uint8_t> Q_bytes(x2_bytes.size() + y2_bytes.size());

    std::copy(x2_bytes.begin(), x2_bytes.end(), Q_bytes.begin());
    std::copy(y2_bytes.begin(), y2_bytes.end(), Q_bytes.begin() + x2_bytes.size());
    KDF(Q_bytes, klen, t);
    if (std::all_of(t.begin(), t.end(), [](bool value)
                    { return !value; })) // t 为全 0 比特串
        exit(0);
    std::transform(C.C2.begin(), C.C2.end(), t.begin(), M.begin(), [](bool a, bool b)
                   { return a ^ b; });

    std::vector<uint8_t> M_bytes;
    bits_to_bytes(M, M_bytes);
    std::vector<uint8_t> message(x2_bytes.size() + M_bytes.size() + y2_bytes.size());

    std::vector<uint8_t>::iterator iter = message.begin();
    iter = std::copy(x2_bytes.begin(), x2_bytes.end(), iter);
    iter = std::copy(M_bytes.begin(), M_bytes.end(), iter);
    std::copy(y2_bytes.begin(), y2_bytes.end(), iter);

    uint8_t *hash_val = message.data();
    size_t length = message.size();
    uint8_t digest[SHA512_DIGEST_LENGTH];
    SHA512(hash_val, length, digest);
    std::vector<uint8_t> data_bytes(digest, digest + SHA512_DIGEST_LENGTH);
    bytes_to_bits(data_bytes, u);
    if (!std::equal(u.begin(), u.end(), C.C3.begin())) // u 和 C3 不相等
        exit(0);

    return M;
}