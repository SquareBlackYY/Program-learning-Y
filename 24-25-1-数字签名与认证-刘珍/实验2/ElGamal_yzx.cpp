#include <iostream>
#include <gmpxx.h>
#include <vector>
using namespace std;

// 公钥
struct ElGamal_Public_Key
{
    mpz_class p;
    mpz_class g;
    mpz_class y;

    ElGamal_Public_Key(const mpz_class p, const mpz_class g, const mpz_class y) : p(p), g(g), y(y) {}
};

// 私钥
struct ElGamal_Private_Key
{
    mpz_class p;
    mpz_class g;
    mpz_class x;

    ElGamal_Private_Key(const mpz_class p, const mpz_class g, const mpz_class x) : p(p), g(g), x(x) {}
};

// 密文
struct ElGamal_Signature
{
    mpz_class r;
    mpz_class s;
};

void int_to_bytes(const mpz_class &, vector<uint8_t> &);                                            // 整数转字节串
void bytes_to_int(const vector<uint8_t> &, mpz_class &);                                            // 字节串转整数
bool Miller_Rabin(const mpz_class &);                                                               // Miller_Rabin 素性检测
mpz_class get_prime(const size_t &);                                                                // 生成 bitLength 位二进制长度的随机素数
mpz_class get_random_number(const mpz_class &, const mpz_class &);                                  // 生成指定范围 [min, max] 内的随机整数
mpz_class pow_mod(const mpz_class &, const mpz_class &, const mpz_class &);                         // 快速指数幂模运算
mpz_class gcd(mpz_class, mpz_class);                                                                // 最大公约数
void exgcd(const mpz_class &, const mpz_class &, mpz_class *);                                      // 扩展欧几里得算法
mpz_class get_inv(mpz_class , const mpz_class &);                                                   // 求逆
bool is_primitive_root(const mpz_class &, const mpz_class &, const vector<mpz_class> &);            // 判断是否是本原根
mpz_class get_primitive_root(const mpz_class &);                                                    // 得到本原根
ElGamal_Signature ElGamal_sig(const mpz_class &, const ElGamal_Private_Key &);                      // ElGamal 签名
bool ElGamal_verify(const ElGamal_Signature &, const ElGamal_Public_Key &, const mpz_class &);      // ElGamal 验签
bool ElGamal_reuse_random(const mpz_class &, const mpz_class &, const ElGamal_Private_Key &);       // ElGamal 随机数重用攻击

// 生成公私钥
class ElGamal_Key_gen
{
private:
    mpz_class p;
    mpz_class g;
    mpz_class y;
    mpz_class x;

public:
    ElGamal_Key_gen(const size_t &bitLength = 512, const mpz_class &p_val = 0, const mpz_class &g_val = 0, const mpz_class &x_val = 0)
                        :p(p_val), g(g_val), x(x_val)
    {
        if (p_val == 0) {     // 若用户没有给定 p 则生成
            p = get_prime(bitLength);
        }

        if (g_val == 0) {
            g = get_primitive_root(p);
        }

        if (x_val == 0) {
            x = get_random_number(1, p - 2);
        }
        
        mpz_powm(y.get_mpz_t(), g.get_mpz_t(), x.get_mpz_t(), p.get_mpz_t());
    }

    ElGamal_Public_Key get_public_key() {
        return ElGamal_Public_Key(p, g, y);
    }
    ElGamal_Private_Key get_private_key() {
        return ElGamal_Private_Key(p, g, x);
    }
};

int main()
{
    ElGamal_Key_gen key(32, mpz_class("0xaa5ae5a2ff388b78174378cfdea0f7363893a63c68227df45ed8be2cde31241f"), 
                        mpz_class("0x5"),
                        mpz_class("0x4A5B6C7D8E9F0A1B2C3D4E5F6A7B8C9"));
    const ElGamal_Public_Key pk = key.get_public_key();
    const ElGamal_Private_Key sk = key.get_private_key();

    string message = "This is a test message for ElGamal signature";
    cout << hex << "明文消息:\t\t" << message << endl;

    vector<uint8_t> m_bytes(message.begin(), message.end());   // 明文字符串转字节串
    mpz_class m;
    bytes_to_int(m_bytes, m);
    cout << "明文字节串：\t\t"  << m << endl;

    ElGamal_Signature sig = ElGamal_sig(m, sk);
    cout << "签名结果：\t\t(" << sig.r << ", " << sig.s << ")" << endl;
    
    if (ElGamal_verify(sig, pk, m)) {
        cout << "验签成功" << endl;
    } else {
        cout << "验签失败" << endl;
    }

    mpz_class m2("0xffffffffffffffff111111111111");
    if (ElGamal_reuse_random(m, m2, sk)) {
        cout << "随机数重用成功恢复私钥" << endl;
    } else {
        cout << "随机数重用攻击失败" << endl;
    }
    
    return 0;
}


// 整数转字节串
void int_to_bytes(const mpz_class &value, vector<uint8_t> &bytes) {
    size_t numBytes = mpz_sizeinbase(value.get_mpz_t(), 256);   // 计算需要的字节数
    bytes.resize(numBytes);
    for (size_t i = 0; i < numBytes; ++i) {
        mpz_class byte = (value >> ((numBytes - i - 1) * 8)) & 0xff;
        bytes[i] |= mpz_get_ui(byte.get_mpz_t());               // 取出每个字节
    }   
}

// 字节串转整数
void bytes_to_int(const vector<uint8_t> &bytes, mpz_class &value) {
    size_t numBytes = bytes.size();
    value = 0;
    for (size_t i = 0; i < numBytes; ++i)
        value = (value << 8) | bytes[i];
}

// Miller_Rabin 素性检测
bool Miller_Rabin(const mpz_class &p) {
    size_t k = 0;
	mpz_class t = p - 1;
	while ((t & 1) == 0) {                      //判断是否为偶数
		t = t >> 1;                             //除二操作
		k++;
	}
    mpz_class a = get_random_number(2, p - 1);  // 在 [2, p - 1] 之间选一个底数
    mpz_class b = pow_mod(a, t, p);
    if (b == 1)
        return true;
    for (size_t j = 0; j <= k; j++) {
        if ((b + 1) % p == 0)
            return true;
        b = b * b % p;
    }
    return false;
}

// 生成 bitLength 位二进制长度的随机素数
mpz_class get_prime(const size_t &bitLength) {
    mpz_class beg, end;
    mpz_ui_pow_ui(beg.get_mpz_t(), 2, bitLength - 1);
    mpz_ui_pow_ui(end.get_mpz_t(), 2, bitLength);
    mpz_class p;
    bool isprime;
    do {
        isprime = true;
        p = get_random_number(beg, end - 1);    // 生成 [2 ^ (bitLength - 1), 2 ^ bitLength - 1] 之间的随机数
        if (p != 2 && (p & 1) == 0)
            p++;
        bool flag = false;
        for (size_t i = 0; i < 6; ++i) {        // 6轮 Miller_Rabin 素性检测
            flag = Miller_Rabin(p);
            if (!flag) {
                isprime = false;
                break;
            }
        }
    } while (!isprime);
    return p;
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

// 快速指数幂模运算
mpz_class pow_mod(const mpz_class &base, const mpz_class &exp, const mpz_class &mod) {
    mpz_class result;
    mpz_powm(result.get_mpz_t(), base.get_mpz_t(), exp.get_mpz_t(), mod.get_mpz_t());
    return result;
}

// 最大公约数
mpz_class gcd(mpz_class a, mpz_class b) {
    mpz_class tmp = 0;
    while (b != 0) {
        tmp = a % b;
        a = b;
        b = tmp;
    }
    return a;
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
        cout << "No inverse!" << endl;
        exit(0);
    }
    else {
        return res[2] < 0 ? res[2] + phi : res[2];
    }
}

// 判断是否是本原根
bool is_primitive_root(const mpz_class &g, const mpz_class &p, const vector<mpz_class> &primes) {
    for (mpz_class q : primes)
        if (pow_mod(g, (p - 1) / q, p) == 1)
            return false;
    return true;
}

// 得到 Z_p^* 的本原根（这里默认输入为素数）
mpz_class get_primitive_root(const mpz_class &p) {
    vector<mpz_class> primes;
    mpz_class phi_p = p - 1, p_sqrt;
    mpz_sqrt(p_sqrt.get_mpz_t(), phi_p.get_mpz_t());

    // 求所有素因子
    for (mpz_class i = 2; i <= p_sqrt; ++i) {
        if (phi_p % i == 0) {
            primes.push_back(i);
            while (phi_p % i == 0)
                phi_p /= i;
        }
    }
    // 若 phi_p 存在一个大于 p_sqrt 的因子，则一定是质因子
    if (phi_p > 1)
        primes.push_back(phi_p);
    
    for (mpz_class g = 2; g < p; ++g)
        if (is_primitive_root(g, p, primes))
            return g;
    return -1;
}

// ElGamal 签名
ElGamal_Signature ElGamal_sig(const mpz_class &m, const ElGamal_Private_Key &sk) {
    ElGamal_Signature sig;
    mpz_class k;
    do {
        k = get_random_number(1, sk.p - 2);
    } while (gcd(k, sk.p - 1) != 1);
    
    sig.r = pow_mod(sk.g, k, sk.p);
    sig.s = (((m - sk.x * sig.r) % (sk.p - 1) + (sk.p - 1)) % (sk.p - 1)) * get_inv(k, sk.p - 1) % (sk.p - 1);
    return sig;
}

// ElGamal 验签
bool ElGamal_verify(const ElGamal_Signature &sig, const ElGamal_Public_Key &pk, const mpz_class &m) {

    if (pow_mod(pk.y, sig.r, pk.p) * pow_mod(sig.r, sig.s, pk.p) % pk.p == pow_mod(pk.g, m, pk.p))
        return true;
    else
        return false;
}

// ElGamal 随机数重用攻击
bool ElGamal_reuse_random(const mpz_class &m1, const mpz_class &m2, const ElGamal_Private_Key &sk) {
    ElGamal_Signature sig1, sig2;
    mpz_class k;
    do {
        k = get_random_number(1, sk.p - 2);
        sig1.r = pow_mod(sk.g, k, sk.p);
    } while (gcd(k, sk.p - 1) != 1 || gcd(sig1.r, sk.p - 1) != 1);
    
    sig1.s = (((m1 - sk.x * sig1.r) % (sk.p - 1) + (sk.p - 1)) % (sk.p - 1)) * get_inv(k, sk.p - 1) % (sk.p - 1);

    sig2.r = pow_mod(sk.g, k, sk.p);
    sig2.s = (((m2 - sk.x * sig2.r) % (sk.p - 1) + (sk.p - 1)) % (sk.p - 1)) * get_inv(k, sk.p - 1) % (sk.p - 1);

    mpz_class d = gcd(sig1.s - sig2.s, sk.p - 1);
    mpz_class m_ = (m1 - m2) / d, s_ = (sig1.s - sig2.s) / d, p_ = (sk.p - 1) / d;

    mpz_class k_guess = m_ * get_inv(s_, p_) % p_;

    // cout << k << endl << k_guess << endl;
    
    mpz_class x_attack = (((m1 - sig1.s * k_guess) % (sk.p - 1) + (sk.p - 1)) % (sk.p - 1)) * get_inv(sig1.r, sk.p - 1) % (sk.p - 1);
    if (x_attack == sk.x)
        return true;
    else
        return false;
}
