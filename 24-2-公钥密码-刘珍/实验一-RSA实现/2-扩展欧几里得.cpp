#include <iostream>
#include <cmath>
using namespace std;

int main()
{
    int p = 5, q = 7;
    int n = p * q, fn = (p - 1) * (q - 1);
    int e = 5;
    int d;
    for (d = 1; d < fn; d++)
        if (d * e % fn == 1)
            break;
    int length = 3;
    char m[length] = {'b', 'e', 'd'};
    int c[length];
    for (int i = 0; i < length; i++)
        c[i] = (static_cast<int>(pow(static_cast<int>(m[i] - 'a') + 1, e)) % n) + 'a' - 1;

    for (int i = 0; i < length; i++)
        cout << static_cast<char>(c[i]) << " ";

    return 0;
}