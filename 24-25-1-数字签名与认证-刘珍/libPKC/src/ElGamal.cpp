//
// Created by 张笑宇 on 2024/10/2.
//

#include "ElGamal.h"

using namespace std;

ElGamal::ElGamal() {
    p = "0xaa5ae5a2ff388b78174378cfdea0f7363893a63c68227df45ed8be2cde31241f";
    g = "0x5";
}

ElGamal::ElGamal(mpz_class p, mpz_class g) : p(std::move(p)), g(std::move(g)) {
}

ElGamal::Signature::Signature() = default;

ElGamal::Signature::Signature(mpz_class r, mpz_class s) : r(std::move(r)), s(std::move(s)) {
}

ElGamal::PrivateKey::PrivateKey() = default;

ElGamal::PrivateKey::PrivateKey(mpz_class x, mpz_class p, mpz_class g) : x(std::move(x)), p(std::move(p)), g(std::move(g)) {
}

ElGamal::Signature ElGamal::PrivateKey::sign(const string &m_str) const {
    const mpz_class m = stringToMpz(m_str);

    const mpz_class k = getRandNumWithCoprime(1, p - 1, p - 1);

    const mpz_class r = powm(g, k, p);
    const mpz_class s = getInv(k, p - 1) * (((m - x * r) % (p - 1) + (p - 1)) % (p - 1)) % (p - 1);

    return {r, s};
}

bool ElGamal::PrivateKey::signReuseRandNum(const string &m_str_1, const string &m_str_2) const {
    const mpz_class m_1 = stringToMpz(m_str_1);
    const mpz_class m_2 = stringToMpz(m_str_2);

    mpz_class k ,r;
    do {
        k = getRandNumWithCoprime(1, p - 1, p - 1);
        r = powm(g, k, p);
    } while (!isCoprime(r, p - 1));

    cout << "生成的随机数 k : " << k << endl;
    const mpz_class s_1 = getInv(k, p - 1) * (((m_1 - x * r) % (p - 1) + (p - 1)) % (p - 1)) % (p - 1);
    const mpz_class s_2 = getInv(k, p - 1) * (((m_2 - x * r) % (p - 1) + (p - 1)) % (p - 1)) % (p - 1);

    // 攻击参数计算
    const mpz_class d = gcd(s_1 - s_2, p - 1);

    const mpz_class m_prime = ((m_1 - m_2) % (p - 1) + (p - 1)) % (p - 1) / d;
    const mpz_class s_prime = ((s_1 - s_2) % (p - 1) + (p - 1)) % (p - 1) / d;
    const mpz_class p_prime = (p - 1) / d;

    const mpz_class k_guess = m_prime * getInv(s_prime, p_prime) % p_prime;
    cout << "计算的随机数 k : " << k_guess << endl;

    const mpz_class x_attack = ((m_1 - s_1 * k_guess) % (p - 1) + (p - 1)) % (p - 1) * getInv(r, p - 1) % (p - 1);
    cout << "恢复的私钥 x :" << x_attack << endl;

    return x_attack == x;
}

ElGamal::PrivateKey ElGamal::getPrivateKey() const {
    return {getRandNum(1, p - 1), p, g};
}

ElGamal::PrivateKey ElGamal::getPrivateKey(const mpz_class &x) const {
    return {x, p, g};
}

ElGamal::PublicKey::PublicKey() = default;

ElGamal::PublicKey::PublicKey(mpz_class y, mpz_class p, mpz_class g) : y(std::move(y)), p(std::move(p)), g(std::move(g)) {
}

bool ElGamal::PublicKey::verify(const Signature &sig, const string &m_str) const {
    const mpz_class m = stringToMpz(m_str);

    return powm(y, sig.r, p) * powm(sig.r, sig.s, p) % p == powm(g, m, p);
}

ElGamal::PublicKey ElGamal::getPublicKey(const PrivateKey &sk) const {
    return {powm(g, sk.x, p), p, g};
}

ElGamal::PublicKey ElGamal::setPublicKey(const mpz_class &y) const {
    return {y, p, g};
}

ostream &operator<<(ostream &os, const ElGamal &param) {
    os << "ElGamal参数:" << endl;
    os << "p : " << param.p << endl;
    os << "g : " << param.g << endl;
    return os;
}

ostream &operator<<(ostream &os, const ElGamal::Signature &sig) {
    os << "ElGamal签名:" << endl;
    os << "r : " << sig.r << endl;
    os << "s : " << sig.s << endl;
    return os;
}

ostream &operator<<(ostream &os, const ElGamal::PrivateKey &sk) {
    os << "ElGamal私钥:" << endl;
    os << "x : " << sk.x << endl;
    return os;
}

ostream &operator<<(ostream &os, const ElGamal::PublicKey &pk) {
    os << "ElGamal公钥:" << endl;
    os << "y : " << pk.y << endl;
    return os;
}