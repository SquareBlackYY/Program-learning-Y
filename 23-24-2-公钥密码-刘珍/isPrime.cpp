#include <iostream>
#include <gmpxx.h>

using namespace std;

bool isPrime(const mpz_class& num)
{
    if (num <= 1)
        return false;
    
    if (num <= 3)
        return true;
    
    mpz_class sqrt_num;
    mpz_sqrt(sqrt_num.get_mpz_t(), num.get_mpz_t());
    
    for (mpz_class i = 2; i <= sqrt_num; i++)
        if (num % i == 0)
            return false;
    
    return true;
}

int main()
{
    mpz_class number("120831328589021067656590476198673127325963060078239792016790498030350403400124670632041185580708648592318723236563505677830795814941947565377760957329581693313813427282841236806535982108921499859258621755464814791145110416092019488446921944136827349001705136995574997562152840621637152113458959880473732920829");
    
    if (isPrime(number))
        cout << "素数" << endl;
    else
        cout << "非素数" << endl;
    
    return 0;
}