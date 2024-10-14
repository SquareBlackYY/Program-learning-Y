#include <openssl/sha.h>
#include <iostream>
#include <iomanip>
#include <cstdint>
#include <vector>
#include <gmpxx.h>


class SM2_Signature;
class Point;
class Elliptic_Curve;
class SM2_Key_gen;

mpz_class get_random_number(const mpz_class &, const mpz_class &);                                                  // 生成指定范围 [min, max] 内的随机整数
void exgcd(const mpz_class &, const mpz_class &, mpz_class *);                                                      // 扩展欧几里得算法
mpz_class get_inv(mpz_class , const mpz_class &);                                                                   // 求逆
mpz_class pow_mod(const mpz_class &, const mpz_class &, const mpz_class &);                                         // 快速指数幂模运算
mpz_class SHA256_to_mpz(const mpz_class &);                                                                         // SHA256 哈希
Point Elliptic_add(const Elliptic_Curve &, const Point &, const Point &);                                           // 椭圆曲线加法
Point Elliptic_mul(const Elliptic_Curve &, const mpz_class, const Point &);                                         // 椭圆曲线倍点

void int_to_bytes(const mpz_class &, std::vector<uint8_t> &);                                                       // 整数转字节串
void bytes_to_int(const std::vector<uint8_t> &, mpz_class &);                                                       // 字节串转整数
SM2_Signature SM2_sig(const Elliptic_Curve &, const mpz_class &, const mpz_class &, const mpz_class &);             // SM2 签名
bool SM2_verify(const Elliptic_Curve &, const Point &, const mpz_class &, const SM2_Signature &, const mpz_class &);// SM2 验签

// 签名
class SM2_Signature
{
public:
    mpz_class r;
    mpz_class s;

    // 重载输出流运算符
    friend std::ostream &operator<<(std::ostream &os, const SM2_Signature &sig) {
        os << "(" << sig.r << ", " << sig.s << ")";
        return os;
    }
};

// 点坐标
class Point {
public:
    mpz_class x;
    mpz_class y;

    Point() : x(0), y(0) {}
    Point(const mpz_class &x_val, const mpz_class &y_val) : x(x_val), y(y_val) {}

    // 判断点是否是无穷远点
    bool is_infinity() const {
        return x == 0 && y == 0;
    }

    // 重载输出流运算符
    friend std::ostream &operator<<(std::ostream &os, const Point &P) {
        os << "(" << P.x << ", " << P.y << ")";
        return os;
    }
};

// 椭圆曲线
class Elliptic_Curve
{
public:
    mpz_class p;
    mpz_class a;
    mpz_class b;
    mpz_class n;
    Point G;
    mpz_class h;

    Elliptic_Curve(const mpz_class &p_val, const mpz_class &a_val, const mpz_class &b_val, const Point &G_point = Point(), const mpz_class &n_val = 0, const mpz_class &h_val = 1)
                    : p(p_val), a(a_val), b(b_val), G(G_point), n(n_val), h(h_val)
    {
        if (n_val == 0) {
            Point R = G;
            n = 1;

            while (!R.is_infinity()) {
                R = Elliptic_add(*this, R, G);
                n++;
            }
        }
    }
};

// 生成公私钥
class SM2_Key_gen
{
public:
    Elliptic_Curve E;
    mpz_class d;
    Point P;

    SM2_Key_gen(const Elliptic_Curve &E_val, const mpz_class &d_val = 0, const Point &P_val = Point()) : E(E_val), d(d_val), P(P_val) {
        if (d_val == 0) {
            do {
                d = get_random_number(1, mpz_class(E.n));
            } while (gcd(1 + d, E.n) != 1);
        }

        if (P_val.is_infinity()) {
            P = Elliptic_mul(E, d, E.G);
        }
    }
};


int main()
{
    Elliptic_Curve E(mpz_class(23), mpz_class(1), mpz_class(1), Point(mpz_class(3), mpz_class(10)));
    SM2_Key_gen K(E);

    std::string message = "This is a test message for SM2 signature";
    std::cout << std::hex << "明文消息:\t\t" << message << std::endl;

    std::vector<uint8_t> m_bytes(message.begin(), message.end());   // 明文字符串转字节串
    mpz_class m;
    bytes_to_int(m_bytes, m);
    std::cout << "明文字节串：\t\t"  << m << std::endl;

    mpz_class e("0x3");

    SM2_Signature sig = SM2_sig(E, K.d, m, e);
    std::cout << "签名结果：\t\t" << sig << std::endl;

    if (SM2_verify(E, K.P, m, sig, e)) {
        std::cout << "验签成功" << std::endl;
    } else {
        std::cout << "验签失败" << std::endl;
    }

    return 0;
}


// 生成指定范围 [min, max] 内的随机整数
mpz_class get_random_number(const mpz_class &min, const mpz_class &max) {
    // 在函数内部使用静态变量保存随机数生成器，确保生成器在多次调用中保持状态
    static gmp_randclass rand(gmp_randinit_default);
    // 只在第一次调用时设置种子
    static bool seed_initialized = false;
    if (!seed_initialized) {
        rand.seed(time(NULL));
        seed_initialized = true;
    }

    mpz_class range = max - min + 1;
    mpz_class random_num = rand.get_z_range(range) + min;
    return random_num;
}

// 扩展欧几里得算法
void exgcd(const mpz_class &f, const mpz_class &d, mpz_class *res) {
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
            res[0] = y[2], res[1] = y[0], res[2] = y[1];
            return;
        }
        Q = x[2] / y[2];
        t[0] = x[0] - Q * y[0],  t[1] = x[1] - Q * y[1], t[2] = x[2] - Q * y[2];
        x[0] = y[0], x[1] = y[1], x[2] =  y[2];
        y[0] = t[0], y[1] = t[1], y[2] = t[2];
    }
}

// 求逆
mpz_class get_inv(mpz_class e, const mpz_class &phi) {
    mpz_class res[3];
    if (e > phi)
        e = e % phi;
    if (e < 0)
        e = e % phi + phi;
    exgcd(phi, e, res);
    if (res[0] != 1) {
        std::cout << "No inverse!" << std::endl;
        exit(0);
    }
    else {
        return res[2] < 0 ? res[2] + phi : res[2];
    }
}

// 快速指数幂模运算
mpz_class pow_mod(const mpz_class &base, const mpz_class &exp, const mpz_class &mod) {
    mpz_class result;
    mpz_powm(result.get_mpz_t(), base.get_mpz_t(), exp.get_mpz_t(), mod.get_mpz_t());
    return result;
}

// SHA256 哈希
mpz_class SHA256_to_mpz(const mpz_class &m) {
    mpz_class e;
    std::vector<uint8_t> hash_bytes;
    
    // 将大整数转为字节数组
    int_to_bytes(m, hash_bytes);

    // 执行SHA256哈希
    uint8_t* hash_val = hash_bytes.data();
    size_t length = hash_bytes.size();
    uint8_t digest[SHA256_DIGEST_LENGTH];
    SHA256(hash_val, length, digest);

    // 将哈希结果转换为字节数组
    std::vector<uint8_t> data_bytes(digest, digest + SHA256_DIGEST_LENGTH);

    // 将字节数组转换为大整数
    bytes_to_int(data_bytes, e);

    return e;
}

// 椭圆曲线加法
Point Elliptic_add(const Elliptic_Curve &E, const Point &P, const Point &Q) {
    if (P.is_infinity())
        return Q;
    if (Q.is_infinity())
        return P;
    if (P.x == Q.x && P.y == E.p - Q.y)
        return Point(mpz_class(0), mpz_class(0));

    mpz_class lambda;
    if (P.x != Q.x)
        lambda = (Q.y - P.y) * get_inv(Q.x - P.x, E.p) % E.p;
    else
        lambda = (3 * P.x * P.x + E.a) * get_inv(2 * P.y, E.p) % E.p;
    Point result;
    result.x = ((lambda * lambda - P.x - Q.x) % E.p + E.p) % E.p;
    result.y = ((lambda * (P.x - result.x) - P.y) % E.p + E.p) % E.p;

    return result;
}

// 椭圆曲线倍点
Point Elliptic_mul(const Elliptic_Curve &E, const mpz_class d, const Point &P) {
    Point Q = P, R(mpz_class(0), mpz_class(0));
    mpz_class n = d;

    while (n > 0) {
        if ((n & 1) == 1)
            R = Elliptic_add(E, R, Q);
        Q = Elliptic_add(E, Q, Q);
        n >>= 1;
    }
    return R;
}

// 整数转字节串
void int_to_bytes(const mpz_class &value, std::vector<uint8_t> &bytes) {
    size_t numBytes = mpz_sizeinbase(value.get_mpz_t(), 256);   // 计算需要的字节数
    bytes.resize(numBytes);
    for (size_t i = 0; i < numBytes; ++i) {
        mpz_class byte = (value >> ((numBytes - i - 1) * 8)) & 0xff;
        bytes[i] |= mpz_get_ui(byte.get_mpz_t());               // 取出每个字节
    }   
}

// 字节串转整数
void bytes_to_int(const std::vector<uint8_t> &bytes, mpz_class &value) {
    size_t numBytes = bytes.size();
    value = 0;
    for (size_t i = 0; i < numBytes; ++i)
        value = (value << 8) | bytes[i];
}

// SM2 签名
SM2_Signature SM2_sig(const Elliptic_Curve &E, const mpz_class &d, const mpz_class &m, const mpz_class &e) {
    mpz_class k;
    Point R;
    SM2_Signature sig;

    while (true) {
        k = get_random_number(1, E.n - 1);
        
        R = Elliptic_mul(E, k, E.G);
        sig.r = (e + R.x) % E.n;

        if (sig.r == 0 || sig.r == E.n - k)
            continue;

        sig.s = get_inv(1 + d, E.n) * (((k - sig.r * d) % E.n) + E.n) % E.n;
        if (sig.s == 0)
            continue;
        break;
    }

    return sig;
}

// SM2 验签
bool SM2_verify(const Elliptic_Curve &E, const Point &P, const mpz_class &m, const SM2_Signature &sig, const mpz_class &e) {
    if (sig.r < 1 || sig.r > E.n - 1 || sig.s < 1 || sig.s > E.n - 1)
        return false;
    
    mpz_class t = (sig.r +sig.s) % E.n;

    if (t == 0)
        return false;

    Point X = Elliptic_add(E, Elliptic_mul(E, sig.s, E.G), Elliptic_mul(E, t, P));
    
    mpz_class R = (e + X.x) % E.n;
    if (R == sig.r)
        return true;
    else
        return false;
}
