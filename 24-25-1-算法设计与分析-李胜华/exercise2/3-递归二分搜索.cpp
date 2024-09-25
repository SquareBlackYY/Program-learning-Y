#include <iostream>
using namespace std;

template <class Type>
int bisearch(Type a[], const Type &x, int low, int high);

int main()
{

    return 0;
}

template <class Type>
int bisearch(Type a[], const Type &x, int low, int high)
{ // 1.
    if (low > high)
        return -1;              // 2.
    int mid = (low + high) / 2; // 3.
    if (x == a[mid])
        return mid; // 4.
    else if (x > a[mid])
        return bisearch(a, x, mid + 1, high); // 5.
    else
        return bisearch(a, x, low, mid - 1); // 6.
}