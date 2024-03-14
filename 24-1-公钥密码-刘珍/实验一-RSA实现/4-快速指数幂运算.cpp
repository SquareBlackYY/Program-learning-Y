#include <iostream>
using namespace std;

long long quick_pow(long long, long long, long long);

int main()
{
    return 0;
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

