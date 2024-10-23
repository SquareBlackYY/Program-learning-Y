#include <iostream>
#include <gmpxx.h>

using namespace std;

mpz_class powm(const mpz_class &base, const mpz_class &power, const mpz_class &mod) {
    mpz_class result;
    mpz_powm(result.get_mpz_t(), base.get_mpz_t(), power.get_mpz_t(), mod.get_mpz_t());
    return result;
}

int main()
{
    mpz_class p = 43, x = 3;
    vector<mpz_class> result(43);
    string s(43, '0');

    for (mpz_class i = 0; i < p; i++)
        if (i % 6 == 0 || i % 6 == 1 || i % 6 == 3)
            result[i.get_ui()] = powm(x, i, p);

    for (mpz_class i = 0; i < p; i++)
        if (result[i.get_ui()] != 0)
            cout << i << "\t" << result[i.get_ui()] << endl;

    for (mpz_class i = 0; i < p; i++)
        if (result[i.get_ui()] != 0)
            s[result[i.get_ui()].get_ui()] = '1';

    cout << s << endl;
    return 0;
}