//
// Created by 张笑宇 on 2024/10/2.
//

#include "DSA_.h"


DSA_::DSA_() {
    p = "0xa030b2bbea795e7533769ff4e6bed8becae8e1f57d80062ed2b38397cc4c110f";
    q = "0x71e886bc4600d3869118146a5abf785911d";
    g = "0x12972b7570fb64952411d8a190995caaf1a573f5141c26b6bb17380a1880d00d";
}

DSA_::DSA_(mpz_class p, mpz_class q, mpz_class g) : p(std::move(p)), q(std::move(q)), g(std::move(g)) {
}

DSA_::Signature::Signature(mpz_class r, mpz_class s) : r(std::move(r)), s(std::move(s)) {
}

DSA_::PrivateKey::PrivateKey(mpz_class x, mpz_class p, mpz_class q, mpz_class g) : x(std::move(x)), p(std::move(p)), q(std::move(q)), g(std::move(g)) {
}

DSA_::Signature DSA_::PrivateKey::sign(const string &m_str) const {
    const mpz_class m = stringToMpz(m_str);

    const mpz_class k = getRandNumWithCoprime(1, q, q);

    const mpz_class r = powm(g, k, p) % q;
    const mpz_class s = (getInv(k, q) * (sha256(m) + x * r)) % q;

    return Signature(r, s);
}

DSA_::PrivateKey DSA_::getPrivateKey() const {
    return PrivateKey(getRandNum(1, q), p, q, g);
}

DSA_::PrivateKey DSA_::getPrivateKey(const mpz_class &x) const {
    return PrivateKey(x, p, q, g);
}

DSA_::PublicKey::PublicKey(mpz_class y, mpz_class p, mpz_class q, mpz_class g) : y(std::move(y)), p(std::move(p)), q(std::move(q)), g(std::move(g)) {
}

bool DSA_::PublicKey::verify(const Signature &sig, const string &m_str) const {
    const mpz_class m = stringToMpz(m_str);

    const mpz_class w = getInv(sig.s, q);
    const mpz_class u1 = (sha256(m) * w) % q;
    const mpz_class u2 = (sig.r * w) % q;
    const mpz_class v = (powm(g, u1, p) * powm(y, u2, p) % p) % q;

    return sig.r == v;
}

DSA_::PublicKey DSA_::getPublicKey(const PrivateKey &sk) const {
    return PublicKey(powm(g, sk.x, p), p, q, g);
}

ostream &operator<<(ostream &os, const DSA_ &param) {
    os << "DSA参数:" << endl;
    os << "p : " << param.p << endl;
    os << "q : " << param.q << endl;
    os << "g : " << param.g << endl;
    return os;
}

ostream &operator<<(ostream &os, const DSA_::Signature &sig) {
    os << "DSA签名:" << endl;
    os << "r : " << sig.r << endl;
    os << "s : " << sig.s << endl;
    return os;
}

ostream &operator<<(ostream &os, const DSA_::PrivateKey &sk) {
    os << "DSA私钥:" << endl;
    os << "x : " << sk.x << endl;
    return os;
}

ostream &operator<<(ostream &os, const DSA_::PublicKey &pk) {
    os << "DSA公钥:" << endl;
    os << "y : " << pk.y << endl;
    return os;
}