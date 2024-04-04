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

    MH_Knapsack_Public_Key(int size, const std::vector<mpz_class> &B, const mpz_class &k) : size(size), B(B), k(k) {}
};

struct MH_Knapsack_Private_Key
{
    int size;
    std::vector<mpz_class> A;
    mpz_class k;
    mpz_class v;

    MH_Knapsack_Private_Key(int size, const std::vector<mpz_class> &A, const mpz_class &k, const mpz_class &v) : size(size), A(A), k(k), v(v) {}
};

void MH_Knapsack_Encrypt(mpz_class &, const mpz_class &, const MH_Knapsack_Public_Key &);
void MH_Knapsack_Encrypt(std::vector<mpz_class> &, const std::string &, const MH_Knapsack_Public_Key &);
void MH_Knapsack_Decrypt(mpz_class &, const mpz_class &, const MH_Knapsack_Private_Key &);
void MH_Knapsack_Decrypt(std::string &, const std::vector<mpz_class> &, const MH_Knapsack_Private_Key &);
void MH_Knapsack_Decrypt_ES(mpz_class &, const mpz_class &, const MH_Knapsack_Public_Key &);
bool isCoprime(const mpz_class &, const mpz_class &);
mpz_class ExEculid(const mpz_class &, const mpz_class &);

// MH背包密钥体系
class MH_Knapsack_Key
{
private:
    int Knapsack_Size;                   // 背包容量
    std::vector<mpz_class> Sequence_SI;  // 超递增背包序列 super-increasing sequence
    std::vector<mpz_class> Sequence_nSI; // 非超递增背包序列 non-super-increasing sequence
    mpz_class s;                         // 超递增背包中所有元素的和 s
    mpz_class k;                         // 模数 k 要求 k > 超递增背包中所有元素的和 s
    mpz_class t;                         // 陷门信息 t 要求 gcd(t, k) == 1
    mpz_class v;                         // 陷门信息的逆 v 要求 v = t ^ -1 mod k

public:
    // 初始化
    MH_Knapsack_Key(int Knapsack_Size = 0, std::vector<mpz_class> Sequence_SI = std::vector<mpz_class>(), mpz_class k = 0, mpz_class t = 0)
    {
        srand(time(0));

        mpz_class s = 0;
        if (Knapsack_Size <= 0 && Sequence_SI.empty())
        {
            std::cout << "没有合适的参数用于生成超递增背包序列" << std::endl;
            exit(1);
        }
        else if (!Sequence_SI.empty())
        {
            Knapsack_Size = Sequence_SI.size();
            for (int i = 0; i < Knapsack_Size; i++)
                s += Sequence_SI[i];
        }
        else
        {
            Sequence_SI.resize(Knapsack_Size);
            s = Sequence_SI[0] = rand();
            for (int i = 1; i < Knapsack_Size; i++)
                s += Sequence_SI[i] = s + rand();
        }
        this->Knapsack_Size = Knapsack_Size;
        this->Sequence_SI = Sequence_SI;
        this->s = s;

        if (k < s)
            k = s + rand();
        this->k = k;
        
        while (!isCoprime(t, k))
            t = rand();
        this->t = t;

        this->v = ExEculid(t, k);

        this->Sequence_nSI.resize(Knapsack_Size);
        for (int i = 0; i < Knapsack_Size; i++)
            this->Sequence_nSI[i] = (t * Sequence_SI[i]) % k;
    }
    // 获取公钥
    MH_Knapsack_Public_Key get_public_key()
    {
        return MH_Knapsack_Public_Key(this->Knapsack_Size, this->Sequence_nSI, this->k);
    }
    // 获取私钥
    MH_Knapsack_Private_Key get_private_key()
    {
        return MH_Knapsack_Private_Key(this->Knapsack_Size, this->Sequence_SI, this->k, this->v);
    }
};

int main()
{
    // 1. 背包密码算法中，超递增序列为(2,3,6,13,27,52)，k=105，t=31，明文数据为011011，实现密文的生成和解密（用穷搜法）。
    MH_Knapsack_Key key1(5, {2, 3, 6, 13, 27, 52}, 105, 31);
    MH_Knapsack_Public_Key pk1 = key1.get_public_key();
    MH_Knapsack_Private_Key sk1 = key1.get_private_key();
    
    mpz_class m = 0b011011;
    std::cout << "明文:\t\t" << m << std::endl;
    
    mpz_class c;
    MH_Knapsack_Encrypt(c, m, pk1);
    std::cout << "加密结果:\t" << c << std::endl;

    mpz_class m_decrypt;
    MH_Knapsack_Decrypt(m_decrypt, c, sk1);
    std::cout << "解密结果:\t" << m_decrypt << std::endl;

    MH_Knapsack_Decrypt_ES(m_decrypt, c, pk1);
    std::cout << "穷搜法解密结果:\t" << m_decrypt << std::endl;

    // 2.利用ppt中的英文字母，空格与数字之间的关系，生成一个长为20的超递增背包，加密消息KNAPSACK PROBLEM后，再解密。
    MH_Knapsack_Key key2(20);
    MH_Knapsack_Public_Key pk2 = key2.get_public_key();
    MH_Knapsack_Private_Key sk2 = key2.get_private_key();

    std::string m_str = "KNAPSACK PROBLEM";
    std::cout << "字符串明文:\t" << m_str << std::endl;

    std::vector<mpz_class> c_str;
    MH_Knapsack_Encrypt(c_str, m_str, pk2);
    std::cout << "加密结果:\t";
    for (mpz_class ch : c_str)
        std::cout << ch << " ";
    std::cout << std::endl;

    std::string m_decrypt_str;
    MH_Knapsack_Decrypt(m_decrypt_str, c_str, sk2);
    std::cout << "解密结果:\t" << m_decrypt_str << std::endl;

    return 0;
}

void MH_Knapsack_Encrypt(mpz_class &c, const mpz_class &m, const MH_Knapsack_Public_Key &pk)
{
    for (int i = 0; i < pk.size; i++)
        c += ((m >> i) & 1) * pk.B[pk.size - i - 1];
}

void MH_Knapsack_Encrypt(std::vector<mpz_class> &c, const std::string &m, const MH_Knapsack_Public_Key &pk)
{
    int block_size = pk.size / 5;
    int c_str_length = (m.size() + block_size - 1) / block_size;
    c.resize(c_str_length);

    mpz_class block;
    for (int i = 0; i < c_str_length; i++)
    {
        block = 0;
        for (int j = 0; j < block_size; j++)
            if (i * block_size + j < m.size() && m[i * block_size + j] != ' ')
                block += (mpz_class)(m[i * block_size + j] - 'A' + 1) << ((block_size - 1 - j) * 5);
        MH_Knapsack_Encrypt(c[i], block, pk);
    }
}

void MH_Knapsack_Decrypt(mpz_class &m, const mpz_class &c, const MH_Knapsack_Private_Key &sk)
{
    m = 0;
    mpz_class s = (sk.v * c) % sk.k;
    for (int i = sk.size - 1; s != 0; i--) 
        if (s - sk.A[i] >= 0)
        {
            m += (mpz_class)1 << (sk.size - i - 1);
            s -= sk.A[i];
        }
}

void MH_Knapsack_Decrypt(std::string &m, const std::vector<mpz_class> &c, const MH_Knapsack_Private_Key &sk)
{
    int block_size = sk.size / 5;
    int m_str_length = c.size() * block_size;
    m.resize(m_str_length);

    mpz_class block, ch;
    for (int i = 0; i < c.size(); i++)
    {
        MH_Knapsack_Decrypt(block, c[i], sk);
        for (int j = block_size - 1; j >= 0; j--)
        {
            ch = ((block >> (j * 5)) & ((1 << 5) - 1));
            m[(i + 1) * block_size - j - 1] = ch.get_ui() == 0 ? ' ' : (char)(ch.get_ui() + 'A' - 1);
        }
    }
}

void MH_Knapsack_Decrypt_ES(mpz_class &m, const mpz_class &c, const MH_Knapsack_Public_Key &pk)
{
    int Max = 1 << pk.size;
    mpz_class result = 0;
    for (m = 0; m < Max && result != c; m++)
    {
        result = 0;
        for (int i = 0; i < pk.size; i++)
            result += ((m >> i) & 1) * pk.B[pk.size - i - 1];
    }
    --m;
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