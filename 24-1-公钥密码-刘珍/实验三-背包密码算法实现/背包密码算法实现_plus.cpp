#include <iostream>
#include <gmpxx.h>
#include <vector>
#include <cstdlib>
#include <ctime>

struct MH_Knapsack_Public_Key
{
    int size;
    std::vector<mpz_class> B;
    mpz_class k;

    MH_Knapsack_Public_Key(const int size, const std::vector<mpz_class> &B, const mpz_class &k) : size(size), B(B), k(k) {}
};

struct MH_Knapsack_Private_Key
{
    int size;
    std::vector<mpz_class> A;
    mpz_class v;

    MH_Knapsack_Private_Key(const int size, const std::vector<mpz_class> &A, const mpz_class &v) : size(size), A(A), v(v) {}
};

mpz_class MH_Knapsack_Encrypt(const mpz_class &, const MH_Knapsack_Public_Key &);
std::vector<mpz_class> MH_Knapsack_Encrypt(const std::string &, const MH_Knapsack_Public_Key &);
mpz_class MH_Knapsack_Decrypt(const mpz_class &, const MH_Knapsack_Public_Key &, const MH_Knapsack_Private_Key &);
std::string MH_Knapsack_Decrypt(const std::vector<mpz_class> &, const MH_Knapsack_Public_Key &, const MH_Knapsack_Private_Key &);
mpz_class MH_Knapsack_Decrypt_ES(const mpz_class &, const MH_Knapsack_Public_Key &);
bool isCoprime(const mpz_class &, const mpz_class &);
mpz_class ExEculid(const mpz_class &, const mpz_class &);

class MH_Knapsack_Key_gen
{
private:
    int Knapsack_Size = 0;                                          // 背包容量
    std::vector<mpz_class> Sequence_SI = std::vector<mpz_class>();  // 超递增背包序列 super-increasing sequence
    std::vector<mpz_class> Sequence_nSI = std::vector<mpz_class>(); // 非超递增背包序列 non-super-increasing sequence
    mpz_class s = 0;                                                // 超递增背包中所有元素的和 s
    mpz_class k = 0;                                                // 模数 k 要求 k > 超递增背包中所有元素的和 s
    mpz_class t = 0;                                                // 陷门信息 t 要求 gcd(t, k) == 1
    mpz_class v = 0;                                                // 陷门信息的逆 v 要求 v = t ^ -1 mod k

public:
    MH_Knapsack_Key_gen(const std::vector<mpz_class> input_Sequence_SI, const mpz_class input_k = 0, const mpz_class input_t = 0)
        : Sequence_SI(input_Sequence_SI), k(input_k), t(input_t)
    {
        if (Sequence_SI.empty())
            exit(1);

        srand(time(0));

        Knapsack_Size = Sequence_SI.size();
        for (int i = 0; i < Knapsack_Size; i++)
            s += Sequence_SI[i];

        if (k < s)
            k = s + rand();

        while (!isCoprime(t, k))
            t = rand();

        v = ExEculid(t, k);

        Sequence_nSI.resize(Knapsack_Size);
        for (int i = 0; i < Knapsack_Size; i++)
            Sequence_nSI[i] = (t * Sequence_SI[i]) % k;
    }
    MH_Knapsack_Key_gen(const int input_Knapsack_Size)
        : Knapsack_Size(input_Knapsack_Size)
    {
        if (Knapsack_Size <= 4)
            exit(1);

        srand(time(0));

        Sequence_SI.resize(Knapsack_Size);

        s = Sequence_SI[0] = rand();
        for (int i = 1; i < Knapsack_Size; i++)
            s += Sequence_SI[i] = s + rand();

        k = s + rand();

        while (!isCoprime(t, k))
            t = rand();

        v = ExEculid(t, k);

        Sequence_nSI.resize(Knapsack_Size);
        for (int i = 0; i < Knapsack_Size; i++)
            Sequence_nSI[i] = (t * Sequence_SI[i]) % k;
    }

    MH_Knapsack_Public_Key get_public_key()
    {
        return MH_Knapsack_Public_Key(Knapsack_Size, Sequence_nSI, k);
    }
    MH_Knapsack_Private_Key get_private_key()
    {
        return MH_Knapsack_Private_Key(Knapsack_Size, Sequence_SI, v);
    }
};

int main()
{
    // 1. 背包密码算法中，超递增序列为(2,3,6,13,27,52)，k=105，t=31，明文数据为011011，实现密文的生成和解密（用穷搜法）。
    MH_Knapsack_Key_gen key1({2, 3, 6, 13, 27, 52}, 105, 31);
    MH_Knapsack_Public_Key pk1 = key1.get_public_key();
    MH_Knapsack_Private_Key sk1 = key1.get_private_key();

    mpz_class m = 0b011011;
    std::cout << "明文:\t\t" << m << std::endl;

    mpz_class c = MH_Knapsack_Encrypt(m, pk1);
    std::cout << "加密结果:\t" << c << std::endl;

    mpz_class m_decrypt = MH_Knapsack_Decrypt(c, pk1, sk1);
    std::cout << "解密结果:\t" << m_decrypt << std::endl;

    m_decrypt = MH_Knapsack_Decrypt_ES(c, pk1);
    std::cout << "穷搜法解密结果:\t" << m_decrypt << std::endl;

    // 2.利用ppt中的英文字母，空格与数字之间的关系，生成一个长为20的超递增背包，加密消息KNAPSACK PROBLEM后，再解密。
    MH_Knapsack_Key_gen key2(20);
    MH_Knapsack_Public_Key pk2 = key2.get_public_key();
    MH_Knapsack_Private_Key sk2 = key2.get_private_key();

    std::string m_str = "KNAPSACK PROBLEM";
    std::cout << "字符串明文:\t" << m_str << std::endl;

    std::vector<mpz_class> c_str = MH_Knapsack_Encrypt(m_str, pk2);
    std::cout << "加密结果:\t";
    for (mpz_class ch : c_str)
        std::cout << ch << " ";
    std::cout << std::endl;

    std::string m_decrypt_str = MH_Knapsack_Decrypt(c_str, pk2, sk2);
    std::cout << "解密结果:\t" << m_decrypt_str << std::endl;

    return 0;
}

mpz_class MH_Knapsack_Encrypt(const mpz_class &m, const MH_Knapsack_Public_Key &pk)
{
    mpz_class c;
    for (int i = 0; i < pk.size; i++)
        c += ((m >> i) & 1) * pk.B[pk.size - i - 1];
    return c;
}

std::vector<mpz_class> MH_Knapsack_Encrypt(const std::string &m, const MH_Knapsack_Public_Key &pk)
{
    const int block_size = pk.size / 5;
    const int c_str_length = (m.size() + block_size - 1) / block_size;
    std::vector<mpz_class> c(c_str_length);

    mpz_class block;
    for (int i = 0; i < c_str_length; i++)
    {
        block = 0;
        for (int j = 0; j < block_size; j++)
            if (i * block_size + j < m.size() && m[i * block_size + j] != ' ')
                block += (mpz_class)(m[i * block_size + j] - 'A' + 1) << ((block_size - 1 - j) * 5);
        c[i] = MH_Knapsack_Encrypt(block, pk);
    }
    return c;
}

mpz_class MH_Knapsack_Decrypt(const mpz_class &c, const MH_Knapsack_Public_Key &pk, const MH_Knapsack_Private_Key &sk)
{
    mpz_class m = 0, s = (sk.v * c) % pk.k;
    for (int i = sk.size - 1; s != 0; i--)
        if (s - sk.A[i] >= 0)
        {
            m += (mpz_class)1 << (sk.size - i - 1);
            s -= sk.A[i];
        }
    return m;
}

std::string MH_Knapsack_Decrypt(const std::vector<mpz_class> &c, const MH_Knapsack_Public_Key &pk, const MH_Knapsack_Private_Key &sk)
{
    const int block_size = sk.size / 5;
    const int m_str_length = c.size() * block_size;
    std::string m(m_str_length, ' ');

    mpz_class block, ch;
    for (int i = 0; i < c.size(); i++)
    {
        block = MH_Knapsack_Decrypt(c[i], pk, sk);
        for (int j = block_size - 1; j >= 0; j--)
        {
            ch = ((block >> (j * 5)) & ((1 << 5) - 1));
            m[(i + 1) * block_size - j - 1] = ch.get_ui() == 0 ? ' ' : (char)(ch.get_ui() + 'A' - 1);
        }
    }
    return m;
}

mpz_class MH_Knapsack_Decrypt_ES(const mpz_class &c, const MH_Knapsack_Public_Key &pk)
{
    const mpz_class Max = (mpz_class)1 << pk.size;
    mpz_class m = 0, result = 0;
    while (result != c && ++m < Max)
    {
        result = 0;
        for (int i = 0; i < pk.size; i++)
            result += ((m >> i) & 1) * pk.B[pk.size - i - 1];
    }
    return m;
}

bool isCoprime(const mpz_class &a, const mpz_class &b)
{
    mpz_class result;
    mpz_gcd(result.get_mpz_t(), a.get_mpz_t(), b.get_mpz_t());
    return !(result.get_ui() - 1);
}

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