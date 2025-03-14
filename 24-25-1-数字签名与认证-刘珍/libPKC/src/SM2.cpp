//
// Created by 张笑宇 on 2024/10/14.
//

#include "SM2.h"

#include <utility>

#include "Schnorr.h"

using namespace std;

SM2::SM2() = default;

SM2::SM2(EllipticCurve E, ECPoint G) : E(std::move(E)), G(std::move(G)) {
    ECPoint R = this->G;
    n = 1;

    while (R != INF_POINT) {
        R += this->G;
        ++n;
    }
}

SM2::SM2(EllipticCurve E, ECPoint G, mpz_class n) : E(std::move(E)), G(std::move(G)), n(std::move(n)) {
}

SM2::Signature::Signature(mpz_class r, mpz_class s) : r(std::move(r)), s(std::move(s)) {
}

SM2::PrivateKey::PrivateKey(mpz_class d, EllipticCurve E, ECPoint G, mpz_class n) : d(std::move(d)), E(std::move(E)),
    G(std::move(G)), n(std::move(n)) {
}

SM2::Signature SM2::PrivateKey::sign(const mpz_class &e) const {
    mpz_class k, x, r, s;

    do {
        do {
            k = getRandNum(1, n);
            x = (k * G).x;
            r = (e + x) % n;
        } while (r == 0 || r == n - k);

        s = getInv(1 + d, n) * ((k - r * d) % n + n) % n;
    } while (s == 0);

    return {r, s};
}

SM2::PrivateKey SM2::getPrivateKey() const {
    return {getRandNumWithCoprime(2, n, n) - 1, E, G, n};
}

SM2::PublicKey::PublicKey(ECPoint P, EllipticCurve E, ECPoint G, mpz_class n) : P(std::move(P)), E(std::move(E)), G(std::move(G)), n(std::move(n)) {}

bool SM2::PublicKey::verify(const Signature &sig, const mpz_class &e) const {
    if (sig.r <= 0 || sig.r >= n || sig.s <= 0 || sig.s >= n) {
        return false;
    }

    const mpz_class t = (sig.r + sig.s) % n;
    if (t == 0) {
        return false;
    }

    const mpz_class x = (sig.s * G + t * P).x;
    const mpz_class R = (e + x) % n;

    return sig.r == R;
}

SM2::PublicKey SM2::getPublicKey(const PrivateKey &sk) const {
    return {sk.d * G, E, G, n};
}

ostream &operator<<(ostream &os, const SM2 &param) {
    os << "SM2参数:" << endl;
    os << "E : " << param.E << endl;
    os << "G : " << param.G << endl;
    os << "n : " << param.n << endl;
    return os;
}

ostream &operator<<(ostream &os, const SM2::Signature &sig) {
    os << "SM2签名:" << endl;
    os << "r : " << sig.r << endl;
    os << "s : " << sig.s << endl;
    return os;
}

ostream &operator<<(ostream &os, const SM2::PrivateKey &sk) {
    os << "SM2私钥:" << endl;
    os << "d : " << sk.d << endl;
    return os;
}

ostream &operator<<(ostream &os, const SM2::PublicKey &pk) {
    os << "SM2公钥:" << endl;
    os << "P : " << pk.P << endl;
    return os;
}
