#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <ctime>
using namespace std;

const int N = 20;
uint64_t table[256];        // 一共 27 个字符，用 5 位二进制表示
const int groupLen = N / 5; // 分组长度

void init();                                                                                    // 初始化映射表
void s2i(string &s, uint64_t *m, int size);                                                     // 字符串转整数数组
void i2s(string &s, uint64_t *m, int size);                                                     // 整数数组转字符串
uint64_t gcd(uint64_t a, uint64_t b);                                                           // 欧几里得算法
void exgcd(int64_t f, int64_t d, int64_t *res);                                                 // 扩展欧几里得算法
uint64_t getInv(uint64_t e, uint64_t phi);                                                      // 求逆
void get_key(uint64_t *A, uint64_t *B, uint64_t &k, uint64_t &t, uint64_t &v, int n);           // 生成长度为 n 的公私钥
void encrypt(uint64_t *m, uint64_t *c, uint64_t *B, int n, int size);                           // 加密
void decrypt(uint64_t *A, uint64_t *c, uint64_t v, uint64_t k, uint64_t *res, int n, int size); // 解密
void BF(uint64_t *B, uint64_t *c, uint64_t *attack, int n, int size);                           // 暴力穷搜
void dfs(uint64_t *B, uint64_t c, string path, uint64_t &attack, int start, int n);             // 穷搜法求解
void print(uint64_t *a, int size);                                                              // 打印函数

int main()
{
    // uint64_t A[6] = {2, 3, 6, 13, 27, 52};
    // uint64_t k = 105, t = 31;
    // uint64_t v = getInv(t, k);
    // cout << "k：" << k <<  " t：" << t << " v：" << v << endl;
    // uint64_t B[6] = {0};
    // for (int i = 0; i < 6; ++i)
    //     B[i] = t * A[i] % k;
    // uint64_t m = 0b011011;
    // cout << "明文为：" << m << endl;
    // uint64_t c = 0;
    // encrypt(&m, &c, B, 6, 1);
    // cout << "加密后密文为：" << c << endl;
    // string s(6, '0');
    // uint64_t attack;
    // dfs(B, c, s, attack, 0, 6);
    // cout << "穷搜法求解明文为：" << attack << endl;

    srand((unsigned)time(NULL));
    init();
    uint64_t A[N], B[N], k, t, v;
    get_key(A, B, k, t, v, N);
    cout << "k：" << k << " t：" << t << " v：" << v << endl;
    cout << "生成的超递增序列私钥为：";
    print(A, N);
    cout << "生成的背包序列公钥为：";
    print(B, N);
    string s = "KNAPSACK PROBLEM";
    uint64_t size = s.length() / groupLen; // 将明文分组，每组的比特长度不超过背包向量个数
    uint64_t m[size] = {0};
    s2i(s, m, size);
    cout << "明文分组后为：";
    print(m, size);
    uint64_t c[size] = {0};
    encrypt(m, c, B, N, size);
    cout << "加密后密文为：";
    print(c, size);
    uint64_t res[size] = {0};
    decrypt(A, c, v, k, res, N, size);
    cout << "解密后的明文为：";
    print(res, size);
    string p;
    i2s(p, res, size);
    cout << "解密后转换为字符串为：" << p << endl;

    uint64_t attack[size];
    BF(B, c, attack, N, size);
    cout << "穷搜法求解明文为：";
    print(attack, size);
    string ans;
    i2s(ans, attack, size);
    cout << "暴力破解后转换为字符串为：" << ans << endl;

    return 0;
}

// 初始化映射表
void init()
{
    table[' '] = 0;
    table[0] = ' ';
    for (int i = 1; i <= 26; ++i)
    {
        table['A' + i - 1] = i;
        table[i] = 'A' + i - 1;
    }
}

// 字符串转整数数组
void s2i(string &s, uint64_t *m, int size)
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < groupLen; ++j)
        {
            if (4 * i + j < s.length())
                m[i] = (m[i] << 5) + table[s[4 * i + j]];
        }
    }
}

// 整数数组转字符串
void i2s(string &s, uint64_t *m, int size)
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < groupLen; ++j)
        {
            s += table[m[i] >> (N - 5 * (j + 1)) & 0x1f];
        }
    }
}

// 欧几里得算法
uint64_t gcd(uint64_t a, uint64_t b)
{
    uint64_t tmp = 0;
    while (b != 0)
    {
        tmp = a % b;
        a = b;
        b = tmp;
    }
    return a;
}

// 扩展欧几里得算法
void exgcd(int64_t f, int64_t d, int64_t *res)
{
    int64_t x[3] = {1, 0, f};
    int64_t y[3] = {0, 1, d};
    int64_t t[3];
    int64_t Q;
    while (true)
    {
        if (y[2] == 0)
        {
            res[0] = x[2];
            return;
        }
        if (y[2] == 1)
        {
            res[0] = y[2], res[1] = y[0], res[2] = y[1];
            return;
        }
        Q = x[2] / y[2];
        t[0] = x[0] - Q * y[0], t[1] = x[1] - Q * y[1], t[2] = x[2] - Q * y[2];
        x[0] = y[0], x[1] = y[1], x[2] = y[2];
        y[0] = t[0], y[1] = t[1], y[2] = t[2];
    }
}

// 求逆
uint64_t getInv(uint64_t e, uint64_t phi)
{
    int64_t res[3];
    if (e > phi)
    {
        e = e % phi;
    }
    exgcd((int64_t)phi, (int64_t)e, res);
    if (res[0] != 1)
    {
        cout << "No inverse!" << endl;
        exit(0);
    }
    else
    {
        return res[2] < 0 ? res[2] + phi : res[2];
    }
}

// 生成长度为 n 的公私钥
void get_key(uint64_t *A, uint64_t *B, uint64_t &k, uint64_t &t, uint64_t &v, int n)
{
    A[0] = rand() % 9 + 1; // 在 [1, 9] 之间随机选一个起始值
    uint64_t sum = A[0];
    for (int i = 1; i < n; ++i)
    {
        A[i] = sum + rand() % 9 + 1;
        sum += A[i];
    }

    k = sum + rand() % 9 + 1;
    do
    {
        t = rand() % (k - 2) + 2; // 在 [2, k - 1] 之间随机选一数
    } while (gcd(t, k) != 1);
    v = getInv(t, k);
    for (int i = 0; i < n; ++i)
        B[i] = t * A[i] % k;
}

// 加密
void encrypt(uint64_t *m, uint64_t *c, uint64_t *B, int n, int size)
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = n - 1; j >= 0; --j)
        {
            c[i] += (m[i] >> (n - j - 1) & 1) * B[j];
        }
    }
}

// 解密
void decrypt(uint64_t *A, uint64_t *c, uint64_t v, uint64_t k, uint64_t *res, int n, int size)
{
    for (int i = 0; i < size; ++i)
    {
        uint64_t sum = v * c[i] % k;
        for (int j = n - 1; j >= 0; --j)
        {
            if (sum >= A[j])
            {
                res[i] += 1 << (n - j - 1);
                sum -= A[j];
            }
        }
    }
}

// 暴力穷搜
void BF(uint64_t *B, uint64_t *c, uint64_t *attack, int n, int size)
{
    for (int i = 0; i < size; ++i)
    {
        string s(n, '0');
        dfs(B, c[i], s, attack[i], 0, N);
    }
}

// 穷搜法求解
void dfs(uint64_t *B, uint64_t c, string path, uint64_t &attack, int start, int n)
{
    if (c <= 0 || start >= n)
    {
        if (c == 0)
            attack = stoull(path, nullptr, 2); // 二进制字符串转无符号长整形
        return;
    }
    for (int i = start; i < n; ++i)
    {
        path[i] = '1';
        dfs(B, c - B[i], path, attack, i + 1, n);
        path[i] = '0';
    }
}

// 打印函数
void print(uint64_t *a, int size)
{
    for (int i = 0; i < size; ++i)
        cout << a[i] << ' ';
    cout << endl;
}