#include <iostream>
#include <cstdlib>
using namespace std;

bool Miller_Rabin(int);
long long quick_pow(long long, long long, long long);

int main()
{
    return 0;
}

// Miller-Rabin素性检测
bool Miller_Rabin(int n)
{
    if (n < 2)
        return false;
    if (n == 2)
        return true;
    if (n % 2 == 0)
        return false;

    long long s = 0, d = n - 1;
    while (d % 2 == 0)
    {
        s++;
        d /= 2;
    }

    // 5轮测试
    for (int i = 0; i < 5; i++)
    {
        long long a = rand() % (n - 1) + 1;
        long long x = quick_pow(a, d, n);
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

// 快速幂取模
long long quick_pow(long long n, long long power, long long mod)
{
    long long ans = 1;
    n %= mod;
    while (power)
    {
        if (power & 1)
            ans = ans * n % mod;
        n = n * n % mod;
        power >>= 1;
    }
    return ans;
}