//
// Created by 张笑宇 on 2024/9/29.
//

#include "EllipticCurve.h"

EllipticCurve::EllipticCurve() = default;

EllipticCurve::EllipticCurve(mpz_class p, mpz_class a, mpz_class b): p(std::move(p)), a(std::move(a)), b(std::move(b)) {}

ostream &operator<<(ostream &os, const EllipticCurve &ec)
{
    os << "y² = x³" ;
    if (ec.a != 0)
        os << (ec.a > 0 ? " + " : " - ") << abs(ec.a) << "x";
    if (ec.b != 0)
        os << (ec.b > 0 ? " + " : " - ") << abs(ec.b);
    return os;
}