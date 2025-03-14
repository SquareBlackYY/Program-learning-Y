//
// Created by 张笑宇 on 2024/9/29.
//

#ifndef ECPOINT_H
#define ECPOINT_H

#include "Tools.h"
#include "EllipticCurve.h"

#define INF_POINT ECPoint(NULL, NULL)

class ECPoint {
public:
    EllipticCurve E;
    mpz_class x;
    mpz_class y;

    ECPoint();

    ECPoint(mpz_class x, mpz_class y);

    ECPoint(EllipticCurve E, mpz_class x, mpz_class y);

    ECPoint operator-() const;

    bool operator==(const ECPoint &another) const;

    bool operator!=(const ECPoint &another) const;

    ECPoint operator+(const ECPoint &another) const;

    void operator+=(const ECPoint &another);
};

ostream &operator<<(ostream &os, const ECPoint &p);

ECPoint operator*(mpz_class multiple, const ECPoint &p);

#endif //ECPOINT_H
