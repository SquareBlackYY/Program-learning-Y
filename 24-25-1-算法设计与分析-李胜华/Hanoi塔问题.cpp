#include <iostream>
using namespace std;

void Hanoi(int, char, char, char);

int main()
{
    Hanoi(3, 'A', 'B', 'C');
    return 0;
}

void Hanoi(int n, char a, char b, char c)
{
    if (n == 1)
        cout << a << "->" << b << endl;
    else
    {
        Hanoi(n - 1, a, c, b);
        cout << a << "->" << b << endl;
        Hanoi(n - 1, c, b, a);
    }
}