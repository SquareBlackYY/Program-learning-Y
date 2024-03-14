#include <iostream>
#include <cmath>
using namespace std;

int ExEuclid(int, int);

int main()
{
    int p = 5, q = 7;
    int n = p * q, fn = (p - 1) * (q - 1);
    int e = 5;
    int d = ExEuclid(fn, e);
    if (d < 0)
        d += fn;
    int length = 3;
    char m[length] = {'b', 'e', 'd'};
    int c[length];
    for (int i = 0; i < length; i++)
        c[i] = (static_cast<int>(pow(static_cast<int>(m[i] - 'a') + 1, e)) % n) + 'a' - 1;

    for (int i = 0; i < length; i++)
        cout << static_cast<char>(c[i]) << " ";
    cout << endl;

    for (int i = 0; i < length; i++)
        cout << static_cast<char>(static_cast<int>(pow(static_cast<int>(c[i] - 'a') + 1, d)) % n + 'a' - 1) << " ";
    cout << endl;
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
            return y[1];
        else if (y[2] == 0)
            return 0;
    }
}