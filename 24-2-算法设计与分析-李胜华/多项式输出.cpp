#include <iostream>
using namespace std;

void print(int, int, int);

int main(){
    int n, a;
    cin >> n;
    for (int i = n; i >= 0; --i){
        int a;
        cin >> a;
        print(a, i, n);
    }
    cout << endl;
    return 0;
}

void print(int a, int i, int n){
    if(a > 0){
        if (i != n) cout << "+";
        if (a != 1 || i == 0) cout << a;
    } else if (a < 0){
        if (a != -1 || i == 0) cout << a;
    } else cout << "-";
    if (a){
        if (i > 1) cout << "x^" << i;
        else if (i == 1) cout << "x";
    }
}