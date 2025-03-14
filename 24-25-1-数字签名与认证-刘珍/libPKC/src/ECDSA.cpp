//
// Created by 张笑宇 on 2024/9/29.
//

#include "ECDSA.h"

ECDSA::ECDSA() {
    E = EllipticCurve(23, 1, 4);
    G = ECPoint(E, 0, 2);

    ECPoint point = INF_POINT;
    do {
        point += G;
        ++n;
    } while (point != INF_POINT);
}

ECDSA::ECDSA(EllipticCurve E, ECPoint G, mpz_class n) : E(std::move(E)), G(std::move(G)), n(std::move(n)) {
}

ECDSA::Signature::Signature(mpz_class r, mpz_class s) : r(std::move(r)), s(std::move(s)) {
}

ECDSA::PrivateKey::PrivateKey(mpz_class d, ECPoint G, mpz_class n) : d(std::move(d)), G(std::move(G)), n(std::move(n)) {
}

ECDSA::Signature ECDSA::PrivateKey::sign(const string &m_str) const {
    const mpz_class m = stringToMpz(m_str);

    const mpz_class e = sha256(m);

    mpz_class k, r, s;

    do {
        do {
            k = getRandNum(1, n);
            ECPoint point = k * G;
            r = point.x % n;
        } while (r == 0);
        s = getInv(k, n) * (e + d * r) % n;
    } while (s == 0);

    return Signature(r, s);
}

ECDSA::PrivateKey ECDSA::getPrivateKey() const {
    return PrivateKey(getRandNum(1, n), G, n);
}

ECDSA::PublicKey::PublicKey(ECPoint Q, ECPoint G, mpz_class n) : Q(std::move(Q)), G(std::move(G)), n(std::move(n)) {
}

bool ECDSA::PublicKey::verify(const Signature &sig, const string &m_str) const {
    if (sig.r < 1 || sig.r >= n || sig.s < 1 || sig.s >= n)
        return false;

    const mpz_class m = stringToMpz(m_str);

    const mpz_class e = sha256(m);

    const mpz_class w = getInv(sig.s, n);

    const mpz_class u1 = e * w % n;
    const mpz_class u2 = sig.r * w % n;

    const ECPoint X = u1 * G + u2 * Q;

    if (X == INF_POINT)
        return false;
    else if (X.x % n == sig.r)
        return true;
    return false;
}

ECDSA::PublicKey ECDSA::getPublicKey(const ECDSA::PrivateKey &sk) const {
    return PublicKey(sk.d * G, G, n);
}

ostream &operator<<(ostream &os, const ECDSA &param) {
    os << "ECDSA参数:" << endl;
    os << "椭圆曲线 E : " << param.E << endl;
    os << "基点 G : " << param.G << endl;
    os << "基点的阶 n : " << param.n << endl;
    return os;
}

ostream &operator<<(ostream &os, const ECDSA::Signature &sig) {
    os << "ECDSA签名:" << endl;
    os << "r : " << sig.r << endl;
    os << "s : " << sig.s << endl;
    return os;
}

ostream &operator<<(ostream &os, const ECDSA::PrivateKey &sk) {
    os << "ECDSA私钥:" << endl;
    os << "d : " << sk.d << endl;
    return os;
}

ostream &operator<<(ostream &os, const ECDSA::PublicKey &pk) {
    os << "ECDSA公钥:" << endl;
    os << "Q : " << pk.Q << endl;
    return os;
}
