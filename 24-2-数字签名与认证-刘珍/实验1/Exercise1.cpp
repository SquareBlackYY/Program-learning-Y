#include <iostream>
#include <gmpxx.h>
#include <cstdlib>
#include <ctime>
#include <openssl/evp.h>
#include <iomanip>
#include <sstream>
using namespace std;

// （1）RSA基本实现
void RSA_key_gen(mpz_class &, mpz_class &, mpz_class &, mpz_class &, mpz_class &);
void RSA_Encrypt(const mpz_class &, mpz_class &, const mpz_class &, const mpz_class &);
void RSA_Decrypt(const mpz_class &, mpz_class &, const mpz_class &, const mpz_class &);

// （2）实现扩展的欧几里得算法
void ExEculid(mpz_class &, const mpz_class &, const mpz_class &);

// （3）实现中国剩余定理
void CRT(const mpz_class &, const mpz_class &, const mpz_class &, const mpz_class &, mpz_class &);

// （4）实现快速指数幂模运算
void quick_pow_mod(mpz_class, mpz_class, const mpz_class &, mpz_class &);

// （5）实现Miller-Rabin算法
bool Miller_Rabin(const mpz_class &);

// （6）RSA快速实现
void RSA_FastDecrypt(const mpz_class &, mpz_class &, const mpz_class &, const mpz_class &, const mpz_class &);

// （7）字节与整数互转
void int_to_bytes(const mpz_class &, string &);
void bytes_to_int(const string &, mpz_class &);

mpz_class generate_random_number(const mpz_class &, const mpz_class &);

string sha256(const string &);

int main()
{
    // 密钥生成
    mpz_class p("8384770691199731090227901761369715679766458105128220592036873364412961378312508844718993092171307186077124445798049353269284692982195512524426286648356027");
    mpz_class q("12181018780270384245980882487763195755475121960663023641593208090932207734556605764818243649182313018177847426957507816306095263109288627131434655751222979");
    mpz_class n("102135049257764615042078588005181805961847617924788520924614970166905740850252377792164156899761196828021256396735835844272953844045954140171199203152941953733292753140561165735333672566395754320988951863125115229868139990823128135019358359614902902066874352974805819176036025088817935594035047801625055544433");
    mpz_class e("65537");
    mpz_class d("66473417932476187344627006008407842157813880631764187733929336672401810736327035450920395324815204991110650019780739766873957814119293344139376700356830267136528754388684199957664206587604561484552338778247510929052310097742136001829518535632056741801490247649680438463755276345908312841043655459326456925849");

    string m, c, m_decrypt;
    mpz_class m_int, c_int, m_decrypt_int;

    cout << "\n1.RSA基础实现" << endl;

    m = "crypto";
    cout << "\n明文:" << m << endl;

    // 加密
    bytes_to_int(m, m_int);
    RSA_Encrypt(m_int, c_int, e, n);
    int_to_bytes(c_int, c);
    cout << "\n密文:" << c << endl;

    // 解密
    RSA_Decrypt(c_int, m_decrypt_int, d, n);
    int_to_bytes(m_decrypt_int, m_decrypt);
    cout << "\n解密结果:" << m_decrypt << endl;

    // 快速解密
    RSA_FastDecrypt(c_int, m_decrypt_int, d, p, q);
    int_to_bytes(m_decrypt_int, m_decrypt);
    cout << "\n快速解密结果:" << m_decrypt << endl;

    cout << "\n2.签名攻击" << endl;
    cout << "\n截获的密文:" << c_int << endl;

    mpz_class r, s, l, t, k, result_int, temp;
    string result;

    do
    {
        r = generate_random_number(2, 128);
        ExEculid(temp, r, n);
    } while (temp == 1);
    cout << "\n生成较小的随机数r:" << r << endl;

    mpz_powm(s.get_mpz_t(), r.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());
    cout << "\n计算s = r ^ e mod n:" << s << endl;
    l = s * c_int % n;
    cout << "\n计算l = s * c mod n:" << l << endl;
    ExEculid(t, r, n);
    cout << "\n计算t = r^-1 mod n:" << t << endl;

    RSA_Encrypt(l, k, d, n);
    cout << "\n设法让签名者对l签名，得到k:" << k << endl;

    result_int = t * k % n;
    int_to_bytes(result_int, result);
    cout << "\n攻击者计算t * k mod n得到明文:" << result << endl;

    cout << "\n3.对哈希值签名与验签" << endl;

    mpz_class hash_int, sig_int, verify_result_int;
    string hash, verify_result;

    hash = sha256(m);
    cout << "\n对明文进行哈希运算，得到哈希值:" << hash << endl;

    bytes_to_int(hash, hash_int);
    RSA_Encrypt(hash_int, sig_int, d, n);
    cout << "\n对哈希值签名，得到签名值:" << sig_int << endl;

    RSA_Decrypt(sig_int, verify_result_int, e, n);
    int_to_bytes(verify_result_int, verify_result);
    cout << "\n验证签名，恢复的哈希值为:" << verify_result << endl;

    cout << "\n验证结果:" << boolalpha << (hash == verify_result) << endl;
    return 0;
}

// （1）RSA简单实现
// 密钥生成
void RSA_key_gen(mpz_class &p, mpz_class &q, mpz_class &n, mpz_class &e, mpz_class &d)
{
    srand(time(0));
    // 生成p, q
    int bit_length = 512;
    int bit_length_diff = 16;

    do
    {
        p = 1;
        for (int i = 1; i < bit_length; i++)
            p = (p << 1) + (rand() % 2);
    } while (!Miller_Rabin(p));

    do
    {
        q = 0;
        for (int i = 1; i < bit_length; i++)
            q = (q << 1) + (rand() % 2);
    } while (!Miller_Rabin(q) || abs((int)(bit_length - mpz_sizeinbase(q.get_mpz_t(), 2))) > bit_length_diff || p == q);

    n = p * q;
    mpz_class fn = (p - 1) * (q - 1);

    // 生成公钥
    e = 65537;

    // 计算私钥
    ExEculid(d, e, fn);
}
// RSA简单加密
void RSA_Encrypt(const mpz_class &m, mpz_class &c, const mpz_class &e, const mpz_class &n)
{
    mpz_powm(c.get_mpz_t(), m.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());
}
// RSA简单解密
void RSA_Decrypt(const mpz_class &c, mpz_class &m, const mpz_class &d, const mpz_class &n)
{
    mpz_powm(m.get_mpz_t(), c.get_mpz_t(), d.get_mpz_t(), n.get_mpz_t());
}

// （2）扩展欧里几得算法求模逆 result = a^-1 mod b
void ExEculid(mpz_class &result, const mpz_class &a, const mpz_class &b)
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

    result = x0;
    if (result < 0)
        result += b;
}

// （3）实现中国剩余定理 x = r_1 mod p, x = r_2 mod q, x = result mod p * q
void CRT(const mpz_class &p, const mpz_class &q, const mpz_class &r_1, const mpz_class &r_2, mpz_class &result)
{
    mpz_class p_inv, q_inv;

    ExEculid(p_inv, p, q);
    ExEculid(q_inv, q, p);

    result = (q_inv * q * r_1 + p_inv * p * r_2) % (p * q);
}

// （4）实现快速指数幂模运算
void quick_pow_mod(mpz_class n, mpz_class power, const mpz_class &mod, mpz_class &result)
{
    result = 1;
    n %= mod;
    while (power > 0)
    {
        if (mpz_odd_p(power.get_mpz_t()))
            result = (result * n) % mod;
        n = (n * n) % mod;
        mpz_fdiv_q_2exp(power.get_mpz_t(), power.get_mpz_t(), 1);
    }
}

// （5）实现Miller-Rabin算法
bool Miller_Rabin(const mpz_class &n)
{
    if (n < 2)
        return false;
    if (n == 2)
        return true;
    if (n % 2 == 0)
        return false;

    mpz_class s = 0, k = n - 1;
    while (k % 2 == 0)
    {
        s++;
        k /= 2;
    }

    // 执行轮数
    int round = 5;
    for (int i = 0; i < round; i++)
    {
        mpz_class a = rand() % (n - 1) + 1, x;
        quick_pow_mod(a, k, n, x);
        if (x == 1 || x == n - 1)
            continue;
        for (int j = 1; j < s; j++)
        {
            x = x * x % n;
            if (x == 1)
                return false;
            if (x == n - 1)
                break;
        }
        if (x != n - 1)
            return false;
    }

    return true;
}

// （6）RSA解密快速实现
void RSA_FastDecrypt(const mpz_class &c, mpz_class &m, const mpz_class &d, const mpz_class &p, const mpz_class &q)
{
    mpz_class v_p, v_q;

    mpz_powm(v_p.get_mpz_t(), c.get_mpz_t(), d.get_mpz_t(), p.get_mpz_t());
    mpz_powm(v_q.get_mpz_t(), c.get_mpz_t(), d.get_mpz_t(), q.get_mpz_t());

    CRT(m, p, q, v_p, v_q);
}

// （7）字节与整数互转
// 整数转字节串
void int_to_bytes(const mpz_class &value, string &bytes)
{
    size_t numOfBytes = (mpz_sizeinbase(value.get_mpz_t(), 2) + 7) / 8;
    bytes.resize(numOfBytes);
    mpz_class temp = value;

    for (size_t i = 0; i < numOfBytes; ++i)
    {
        bytes[numOfBytes - i - 1] = mpz_get_ui(temp.get_mpz_t()) & 0xFF;
        temp >>= 8;
    }
}

// 字节串转整数
void bytes_to_int(const string &bytes, mpz_class &value)
{
    size_t numBytes = bytes.size();
    value = 0;
    for (size_t i = 0; i < numBytes; ++i)
        value = (value << 8) | bytes[i];
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

string sha256(const string &str)
{
    // 定义哈希输出缓冲区和长度
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int length = 0;

    // 创建 EVP 上下文
    EVP_MD_CTX *context = EVP_MD_CTX_new();
    if (context == nullptr)
    {
        // 处理上下文创建失败的情况
        return "";
    }

    // 使用 EVP 接口进行哈希计算
    if (EVP_DigestInit_ex(context, EVP_sha256(), nullptr) &&
        EVP_DigestUpdate(context, str.c_str(), str.size()) &&
        EVP_DigestFinal_ex(context, hash, &length))
    {
        // 将哈希结果转换为十六进制字符串
        stringstream ss;
        for (unsigned int i = 0; i < length; ++i)
        {
            ss << hex << setw(2) << setfill('0') << (int)hash[i];
        }

        // 释放 EVP 上下文
        EVP_MD_CTX_free(context);
        return ss.str();
    }

    // 如果哈希计算失败，释放上下文并返回空字符串
    EVP_MD_CTX_free(context);
    return "";
}