//
// Created by 张笑宇 on 2024/9/29.
//

#include "ECPoint.h"

ECPoint::ECPoint() {}

ECPoint::ECPoint(mpz_class x, mpz_class y) : x(std::move(x)), y(std::move(y)) {}

ECPoint::ECPoint(EllipticCurve E, mpz_class x, mpz_class y) : E(std::move(E)), x(std::move(x)), y(std::move(y)) {}


ECPoint ECPoint::operator-() const {
    return {E, x, -y};
}

bool ECPoint::operator==(const ECPoint &another) const {
    if (E.p != 0)
        return ((x % E.p + E.p) % E.p == (another.x % E.p + E.p) % E.p && (y % E.p + E.p) % E.p == (another.y % E.p + E.p) % E.p);
    else
        return (x == another.x && y == another.y);
}

bool ECPoint::operator!=(const ECPoint &another) const {
    if (E.p != 0)
        return ((x % E.p + E.p) % E.p != (another.x % E.p + E.p) % E.p || (y % E.p + E.p) % E.p != (another.y % E.p + E.p) % E.p);
    else
        return (x != another.x || y != another.y);
}

ECPoint ECPoint::operator+(const ECPoint &another) const {
    mpz_class lambda;
    if (*this == INF_POINT)
        return another;
    else if (another == INF_POINT)
        return *this;
    else if (*this == -another)
        return INF_POINT;
    else if (*this == another)
        lambda = (3 * x * x + E.a) * getInv(2 * y, E.p) % E.p;
    else
        lambda = ((another.y - y) % E.p + E.p) % E.p * getInv(another.x - x, E.p) % E.p;

    ECPoint sum;
    sum.x = ((lambda * lambda - x - another.x) % E.p + E.p) % E.p;
    sum.y = ((lambda * (x - sum.x) - y) % E.p + E.p) % E.p;
    sum.E = E;
    return sum;
}

void ECPoint::operator+=(const ECPoint &another) {
    *this = *this + another;
}

ostream &operator<<(ostream &os, const ECPoint &p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}

ECPoint operator*(mpz_class multiple, const ECPoint &p) {
    ECPoint temp = p, result = INF_POINT;
    while (multiple != 0) {
        if ((multiple & 1) == 1)
            result += temp;
        temp += temp;
        multiple >>= 1;
    }
    return result;
}
