//
// Created by 张笑宇 on 2024/10/2.
//

#ifndef ELGAMAL_H
#define ELGAMAL_H

#include "Tools.h"

class ElGamal {
public:
    mpz_class p;
    mpz_class g;

    ElGamal();

    ElGamal(mpz_class p, mpz_class g);

    class Signature {
    public:
        mpz_class r;
        mpz_class s;

        Signature();

        Signature(mpz_class r, mpz_class s);
    };

    class PrivateKey {
    public:
        mpz_class x;

        mpz_class p;
        mpz_class g;

        PrivateKey();

        PrivateKey(mpz_class x, mpz_class p, mpz_class g);

        Signature sign(const std::string &m_str) const;

        bool signReuseRandNum(const std::string &m_str_1, const std::string &m_str_2) const;
    };

    PrivateKey getPrivateKey() const;

    PrivateKey getPrivateKey(const mpz_class &x) const;

    class PublicKey {
    public:
        mpz_class y;

        mpz_class p;
        mpz_class g;

        PublicKey();

        PublicKey(mpz_class y, mpz_class p, mpz_class g);

        bool verify(const Signature &sig, const std::string &m_str) const;
    };

    PublicKey getPublicKey(const PrivateKey &sk) const;

    PublicKey setPublicKey(const mpz_class &y) const;
};

ostream &operator<<(ostream &os, const ElGamal &param);

ostream &operator<<(ostream &os, const ElGamal::Signature &sig);

ostream &operator<<(ostream &os, const ElGamal::PrivateKey &sk);

ostream &operator<<(ostream &os, const ElGamal::PublicKey &pk);

#endif //ELGAMAL_H
