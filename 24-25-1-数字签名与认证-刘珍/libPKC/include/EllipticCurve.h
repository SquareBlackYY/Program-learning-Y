//
// Created by 张笑宇 on 2024/9/29.
//

#ifndef ELLIPTICCURVE_H
#define ELLIPTICCURVE_H

#include "Tools.h"

class EllipticCurve {
public:
    mpz_class p;
    mpz_class a;
    mpz_class b;

    EllipticCurve();

    EllipticCurve(mpz_class p, mpz_class a, mpz_class b);

};

ostream &operator<<(ostream &os, const EllipticCurve &ec);

#endif //ELLIPTICCURVE_H
