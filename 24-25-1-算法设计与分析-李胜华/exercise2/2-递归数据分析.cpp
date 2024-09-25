#include <iostream>
using namespace std;

void fun(int);

int main()
{
    fun(4);
    return 0;
}

void fun(int n)
{
    if (n > 0)
    {
        cout << n << endl;
        fun(n - 1);
        cout << n << endl;
    }
}