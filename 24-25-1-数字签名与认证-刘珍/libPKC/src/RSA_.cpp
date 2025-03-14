//
// Created by 张笑宇 on 2024/10/2.
//

#include "RSA_.h"

#include <utility>

RSA_::RSA_() {
    p = "8384770691199731090227901761369715679766458105128220592036873364412961378312508844718993092171307186077124445798049353269284692982195512524426286648356027";
    q = "12181018780270384245980882487763195755475121960663023641593208090932207734556605764818243649182313018177847426957507816306095263109288627131434655751222979";
    n = p * q;
}

RSA_::RSA_(mpz_class p, mpz_class q, mpz_class n) : p(std::move(p)), q(std::move(q)), n(std::move(n)) {
}

RSA_::PublicKey::PublicKey(mpz_class e, mpz_class n) : e(std::move(e)), n(std::move(n)) {
}

mpz_class RSA_::PublicKey::encrypt(const string &m_str) const {
    const mpz_class m = stringToMpz(m_str);

    return powm(m, e, n);
}

string RSA_::PublicKey::verify(const mpz_class &sig) const {
    return mpzToString(powm(sig, e, n));
}

bool RSA_::PublicKey::verifyHash(const mpz_class &sig, const string &m_str) const {
    const mpz_class m = stringToMpz(m_str);

    return sha256(m) == powm(sig, e, n);
}

RSA_::PublicKey RSA_::getPublicKey() const {
    return PublicKey(mpz_class(65537), n);
}

RSA_::PublicKey RSA_::setPublicKey(const mpz_class &e) const {
    return PublicKey(e, n);
}

RSA_::PrivateKey::PrivateKey(mpz_class d, mpz_class p, mpz_class q, mpz_class n) : d(std::move(d)), p(std::move(p)), q(std::move(q)), n(std::move(n)) {
}

string RSA_::PrivateKey::decrypt(const mpz_class &c) const {
    return mpzToString(powm(c, d, n));
}

string RSA_::PrivateKey::decryptWithCRT(const mpz_class &c) const {
    const mpz_class v_p = powm(c, d, p);
    const mpz_class v_q = powm(c, d, q);
    const mpz_class X_p = q * getInv(q, p);
    const mpz_class X_q = p * getInv(p, q);
    return mpzToString((v_p * X_p + v_q * X_q) % n);
}

mpz_class RSA_::PrivateKey::sign(const string &m_str) const {
    const mpz_class m = stringToMpz(m_str);

    return powm(m, d, n);
}

mpz_class RSA_::PrivateKey::signHash(const string &m_str) const {
    const mpz_class m = stringToMpz(m_str);

    return powm(sha256(m), d, n);
}

RSA_::PrivateKey RSA_::getPrivateKey(const PublicKey &pk) const {
    return PrivateKey(getInv(pk.e, (p - 1) * (q - 1)), p, q, n);
}

RSA_::PrivateKey RSA_::setPrivateKey(const mpz_class &d) const {
    return PrivateKey(d, p, q, n);
}

ostream &operator<<(ostream &os, const RSA_ &param) {
    os << "RSA参数:" << endl;
    os << "p : " << param.p << endl;
    os << "q : " << param.q << endl;
    os << "n : " << param.n << endl;
    return os;
}

ostream &operator<<(ostream &os, const RSA_::PublicKey &pk) {
    os << "RSA公钥:" << endl;
    os << "e : " << pk.e << endl;
    return os;
}

ostream &operator<<(ostream &os, const RSA_::PrivateKey &sk) {
    os << "RSA私钥:" << endl;
    os << "d : " << sk.d << endl;
    return os;
}

