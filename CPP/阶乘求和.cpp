#include <iostream>
using namespace std;

int fun1(int n);
void fun2(int n, int &res);
int fun3(int n);

int main()
{
    int n, res = 0;
    cin>>n;
    cout<<fun1(n)<<endl;
    fun2(n, res);
    cout<<res<<endl;
    cout<<fun3(n)<<endl;
    return 0;
}

int fun1(int n)
{
    int i = 1, j = 1, res = 0;
    for(; i <= n; i++)
    {
        j *= i;
        res += j;
    }
    return res;
}

void fun2(int n, int &res)
{
    int i = 1, j = 1;
    for(; i <= n; i++)
    {
        j *= i;
        res += j;
    }
}

int fun3(int n)
{
    int i, j, res = 0, x;
    
    for(i = 1; i <= n; i++)
    {
        for(j = 1, x = 1; j <= i; j++)
        {
            x *= j;
        }
        res += x;
    }
    return res;
}