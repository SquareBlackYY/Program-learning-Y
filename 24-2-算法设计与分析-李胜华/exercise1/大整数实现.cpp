#include <iostream>
using namespace std;

string add(string a, string b);
string minus_(string a, string b);
string multiply(string a, string b);
string divide(string a, string b);

int main()
{
    string a("1234567890");
    string b("1234567890");

    cout << add(a, b) << endl;

    cout << minus_(a, b) << endl;

    cout << multiply(a, b) << endl;

    cout << divide(a, b) << endl;

    return 0;
}