#include <iostream>
#include <gmpxx.h>
#include <vector>
#include <cstdlib>
#include <ctime>

void MH_Knapsack_key_gen(int &, std::vector<mpz_class> &, std::vector<mpz_class> &, mpz_class &, mpz_class &, mpz_class &);
mpz_class MH_Knapsack_Encrypt(const int, const mpz_class &, const std::vector<mpz_class> &);
mpz_class MH_Knapsack_Decrypt(const int, const mpz_class &, const mpz_class &, const mpz_class &, const std::vector<mpz_class> &);
mpz_class MH_Knapsack_Decrypt_ES(const int, const mpz_class &, const std::vector<mpz_class> &);
mpz_class Eculid(const mpz_class &, const mpz_class &);
mpz_class ExEculid(const mpz_class &, const mpz_class &);

int main()
{
    std::vector<mpz_class> A, B;
    mpz_class k, t, v;
    int Knapsack_length;

    // 单一明文
    mpz_class m = 0b011011;
    std::cout << "明文:" << m << std::endl;

    // 密钥生成
    A.insert(A.end(), {2, 3, 6, 13, 27, 52});
    k = 105, t = 31;
    MH_Knapsack_key_gen(Knapsack_length, A, B, k, t, v);
    
    // 加密
    mpz_class c;
    c = MH_Knapsack_Encrypt(Knapsack_length, m, B);
    std::cout << "加密结果:" << c << std::endl;

    // 解密
    mpz_class m_decrypt;
    m_decrypt = MH_Knapsack_Decrypt(Knapsack_length, k, v, c, A);
    std::cout << "解密结果:" << m_decrypt << std::endl;

    // 穷搜法
    m_decrypt = MH_Knapsack_Decrypt_ES(Knapsack_length, c, B);
    std::cout << "穷搜法解密结果:" << m_decrypt << std::endl;

    // 字符串明文
    std::string m_str = "KNAPSACK PROBLEM";
    std::cout << "字符串明文:\t" << m_str << std::endl;
    int str_length = m_str.size();

    // 密钥生成
    Knapsack_length = 20;
    A.resize(0);
    MH_Knapsack_key_gen(Knapsack_length, A, B, k, t, v);

    // 根据背包容量确定分组大小
    int block_size = Knapsack_length / 5;

    // 字符串加密
    std::vector<mpz_class> c_str;
    int c_length = (str_length + block_size - 1) / block_size;
    c_str.resize(c_length);

    mpz_class ch;
    for (int i = 0; i < c_length; i++)
    {
        ch = 0;
        for (int j = 0; j < block_size; j++)
            ch += (((i * block_size + j > str_length - 1) || m_str[i * block_size + j] == ' ') ? 0 : (m_str[i * block_size + j] - 'A' + 1)) << ((3 - j) * 5);
        c_str[i] = MH_Knapsack_Encrypt(Knapsack_length, ch, B);
    }

    std::cout << "加密结果:";
    for (mpz_class ch : c_str)
        std::cout << ch << " ";
    std::cout << std::endl;

    // 字符串解密
    std::string m_decrypt_str;
    mpz_class m_decrypt_block;
    int m_decrypt_length = c_length * block_size;
    m_decrypt_str.resize(m_decrypt_length);

    for (int i = 0; i < c_length; i++)
    {
        m_decrypt_block = MH_Knapsack_Decrypt(Knapsack_length, k, v, c_str[i], A);
        for (int j = block_size - 1; j >= 0; j--)
        {
            m_decrypt = (m_decrypt_block >> (j * 5)) & ((1 << 5) - 1);
            m_decrypt_str[(i + 1) * block_size - j - 1] = m_decrypt.get_ui() == 0 ? ' ' : (char)(m_decrypt.get_ui() + 'A' - 1);
        }
    }

    std::cout << "解密结果:\t" << m_decrypt_str << std::endl;

    return 0;
}

void MH_Knapsack_key_gen(int &Knapsack_length, std::vector<mpz_class> &A, std::vector<mpz_class> &B, mpz_class &k, mpz_class &t, mpz_class &v)
{
    srand(time(0));
    mpz_class s = 0;

    if (A.size() == 0)
    {
        A.resize(Knapsack_length);
        A[0] = rand();
        s += A[0];
        for (int i = 1; i < Knapsack_length; i++)
        {
            A[i] = s + rand();
            s += A[i];
        }
    }
    Knapsack_length = A.size();
    if (s == 0)
        for (int i = 1; i < Knapsack_length; i++)
            s += A[i];
    
    if (k <= s)
    {
        k = s + rand();
        do
        {
            t = rand();
        } while (Eculid(t, k) != 1);
    }

    v = ExEculid(t, k);

    B.clear();
    B.resize(Knapsack_length);
    for (int i = 0; i < Knapsack_length; i++)
        B[i] = (t * A[i]) % k;
}

mpz_class MH_Knapsack_Encrypt(const int Knapsack_length, const mpz_class &m, const std::vector<mpz_class> &B)
{
    mpz_class c = 0;
    for (int i = 0; i < Knapsack_length; i++)
        c += ((m >> i) & 1) * B[Knapsack_length - i - 1];
    return c;
}

mpz_class MH_Knapsack_Decrypt(const int Knapsack_length, const mpz_class &k, const mpz_class &v, const mpz_class &c, const std::vector<mpz_class> &A)
{
    mpz_class m = 0, s = (v * c) % k;
    for (int i = Knapsack_length - 1; s != 0; i--)
        if (s - A[i] >= 0)
        {
            m += 1 << (Knapsack_length - i - 1);
            s -= A[i];
        }
    return m;
}

mpz_class MH_Knapsack_Decrypt_ES(const int Knapsack_length, const mpz_class &c, const std::vector<mpz_class> &B)
{
    int Max = 1 << Knapsack_length;
    mpz_class m, result = 0;
    for (m = 0; m < Max && result != c; m++)
    {
        result = 0;
        for (int i = 0; i < Knapsack_length; i++)
            result += ((m >> i) & 1) * B[Knapsack_length - i - 1];
    }
    return --m;
}

mpz_class Eculid(const mpz_class &a, const mpz_class &b)
{
    mpz_class result;
    mpz_gcd(result.get_mpz_t(), a.get_mpz_t(), b.get_mpz_t());
    return result;
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