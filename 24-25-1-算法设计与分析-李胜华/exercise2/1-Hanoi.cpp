#include <iostream>
using namespace std;

void Hanoi(int, char, char, char);

int main()
{
    int n;
    cin >> n;
    Hanoi(n, 'A', 'B', 'C');
    return 0;
}
void Hanoi(int n, char a, char b, char c)
{
    if(!n)
        return;

    Hanoi(n - 1, a, c, b);
    cout << a << " -> " << b << endl;
    Hanoi(n - 1, c, b, a);
}