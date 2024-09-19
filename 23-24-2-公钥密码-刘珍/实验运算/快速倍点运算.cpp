#include <iostream>
#include <gmpxx.h>
using namespace std;

struct Coordinate
{
    mpz_class x;
    mpz_class y;

    Coordinate() {}
    Coordinate(const mpz_class x, const mpz_class y) : x(x), y(y) {}
};

struct Elliptic_Curve
{
    mpz_class p;
    mpz_class a;
    mpz_class b;

    Elliptic_Curve(const mpz_class p, const mpz_class a, const mpz_class b) : p(p), a{a}, b(b) {}
};

mpz_class ExEculid(const mpz_class &a, const mpz_class &b)
{
    mpz_class a_copy = a, b_copy = b, x0 = 1, y0 = 0, x1 = 0, y1 = 1, q, r, x, y;

    while (b_copy != 0)
    {
        mpz_divmod(q.get_mpz_t(), r.get_mpz_t(), a_copy.get_mpz_t(), b_copy.get_mpz_t());

        x = x0 - q * x1;
        y = y0 - q * y1;

        a_copy = b_copy;
        b_copy = r;
        x0 = x1;
        y0 = y1;
        x1 = x;
        y1 = y;
    }

    return (x0 + b) % b;
}

Coordinate ECC_add(const Elliptic_Curve &E, const Coordinate &p, const Coordinate &q)
{
    mpz_class lambda;
    if (p.x == 0 && p.y == 0)
        return q;
    else if (q.x == 0 && q.y == 0)
        return p;
    else if (p.x == q.x && (p.y % E.p + E.p) % E.p == ((-q.y) % E.p + E.p) % E.p)
        return Coordinate(0, 0);
    else if (p.x == q.x && p.y == q.y)
        lambda = ((3 * p.x * p.x + E.a) * ExEculid(2 * p.y, E.p)) % E.p;
    else
        lambda = ((q.y - p.y) * ExEculid(q.x - p.x, E.p)) % E.p;

    Coordinate sum;
    sum.x = ((lambda * lambda - p.x - q.x) % E.p + E.p) % E.p;
    sum.y = ((lambda * (p.x - sum.x) - p.y) % E.p + E.p) % E.p;
    return sum;
}

int main()
{
    Elliptic_Curve E(23, 1, 4);
    Coordinate point(0, 2), temp = point, result(0, 0);
    mpz_class multiple("29"), bit;
    int length = mpz_sizeinbase(multiple.get_mpz_t(), 2);
    for (int i = 0; i < length; i++)
    {
        bit = (multiple >> i) & 1;
        if (bit)
            result = ECC_add(E, result, temp);
        temp = ECC_add(E, temp, temp);
    }
    cout << "结果:(" << result.x << ", " << result.y << ")" << endl;
}