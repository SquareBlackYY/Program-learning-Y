//
// Created by 张笑宇 on 2024/10/2.
//

#include "Schnorr.h"

Schnorr::Schnorr() {
    p = "0xb9c0faef108e0da9bc6a7fd87b9b837a19c2fa3b2daa3ef276fd87fcb7fe690f";
    q = "0x169f3edc9665f26b65fc6e805e3c997160a388a376e9266cfdbb512e107";
    g = "0x5119a79a9849f5c98659566b890077dfa71a7cb22f92e9089d3462b3b2bc16fa";
}

Schnorr::Schnorr(mpz_class p, mpz_class q, mpz_class g) : p(std::move(p)), q(std::move(q)), g(std::move(g)) {
}

Schnorr::Signature::Signature(mpz_class r, mpz_class s) : r(std::move(r)), s(std::move(s)) {
}

Schnorr::PrivateKey::PrivateKey(mpz_class x, mpz_class p, mpz_class q, mpz_class g) : x(std::move(x)), p(std::move(p)), q(std::move(q)), g(std::move(g)) {
}

Schnorr::Signature Schnorr::PrivateKey::sign(const string &m_str) const {
    const mpz_class m = stringToMpz(m_str);

    const mpz_class k = getRandNum(1, q);
    const mpz_class w = powm(g, k, p);

    const mpz_class r = sha256((w << mpz_sizeinbase(p.get_mpz_t(), 2)) + m);
    const mpz_class s = (k + x * r) % q;

    return {r, s};
}

Schnorr::PrivateKey Schnorr::getPrivateKey() const {
    return {getRandNum(2, q), p, q, g};
}

Schnorr::PrivateKey Schnorr::getPrivateKey(const mpz_class &x) const {
    return {x, p, q, g};
}

Schnorr::PublicKey::PublicKey(mpz_class y, mpz_class p, mpz_class q, mpz_class g) : y(std::move(y)), p(std::move(p)), q(std::move(q)), g(std::move(g)) {
}

bool Schnorr::PublicKey::verify(const Signature &sig, const string &m_str) const {
    const mpz_class m = stringToMpz(m_str);

    const mpz_class w_prime = powm(g, sig.s, p) * powm(getInv(y, p), sig.r, p) % p;
    const mpz_class r_prime = sha256((w_prime << mpz_sizeinbase(p.get_mpz_t(), 2)) + m);

    return sig.r == r_prime;
}

Schnorr::PublicKey Schnorr::getPublicKey(const PrivateKey &sk) const {
    return {powm(g, sk.x, p), p, q, g};
}

ostream &operator<<(ostream &os, const Schnorr &param) {
    os << "Schnorr参数:" << endl;
    os << "p : " << param.p << endl;
    os << "q : " << param.q << endl;
    os << "g : " << param.g << endl;
    return os;
}

ostream &operator<<(ostream &os, const Schnorr::Signature &sig) {
    os << "Schnorr签名:" << endl;
    os << "r : " << sig.r << endl;
    os << "s : " << sig.s << endl;
    return os;
}

ostream &operator<<(ostream &os, const Schnorr::PrivateKey &sk) {
    os << "Schnorr私钥:" << endl;
    os << "x : " << sk.x << endl;
    return os;
}

ostream &operator<<(ostream &os, const Schnorr::PublicKey &pk) {
    os << "Schnorr公钥:" << endl;
    os << "y : " << pk.y << endl;
    return os;
}