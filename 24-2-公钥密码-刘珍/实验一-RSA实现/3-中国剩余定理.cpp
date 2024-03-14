#include <iostream>
#include <cmath>
using namespace std;

#define length 3

int ExEuclid(int, int);
void RSA_encrypt(int [], int [], int, int);
void RSA_decrypt(int [], int, int, int, int);

int main()
{
    int p = 5, q = 7;
    int n = p * q, fn = (p - 1) * (q - 1);
    int e = 5;
    int d = ExEuclid(fn, e);
    int m[length] = {'b', 'e', 'd'};
    int c[length];
    
    RSA_encrypt(m, c, e, n);

    for (int i = 0; i < length; i++)
        cout << static_cast<char>(c[i] + 'a' - 1) << " ";
    cout << endl;

    RSA_decrypt(c, d, e, p, q);
    
    return 0;
}

int ExEuclid(int f, int d)
{
    int x[3] = {1, 0, f}, y[3] = {0, 1, d}, t[3];
    int q, i;
    while (1)
    {
        q = x[2] / y[2];
        for (i = 0; i < 3; i++)
            t[i] = x[i] - q * y[i];
        for (i = 0; i < 3; i++)
            x[i] = y[i];
        for (i = 0; i < 3; i++)
            y[i] = t[i];
        if (y[2] == 1)
            return y[1] < 0 ? y[1] : y[1] + f;
        else if (y[2] == 0)
            return 0;
    }
}

void RSA_encrypt(int m[], int c[], int e, int n)
{
    for (int i = 0; i < length; i++)
        c[i] = static_cast<int>(pow(static_cast<int>(m[i] - 'a') + 1, e)) % n;
}

void RSA_decrypt(int c[], int d, int e, int p, int q)
{
    int m[length];
    int q_inv = ExEuclid(p, q), dp = ExEuclid(p - 1, e), dq = ExEuclid(q - 1, e), m1, m2, h;
    for (int i = 0; i < length; i++)
    {
        m1 = static_cast<int>(pow(c[i] , dp)) % p;
        m2 = static_cast<int>(pow(c[i] , dq)) % q;
        h = (q_inv * abs(m1 - m2)) % p;
        m[i] = m2 + h * q;
    }
    for (int i = 0; i < length; i++)
        cout << static_cast<char>(m[i] + 'a' - 1) << " ";
    cout << endl;
}