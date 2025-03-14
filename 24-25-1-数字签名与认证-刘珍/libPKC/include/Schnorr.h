//
// Created by 张笑宇 on 2024/10/2.
//

#ifndef SCHNORR_H
#define SCHNORR_H

#include "Tools.h"

class Schnorr {
public:
    mpz_class p;
    mpz_class q;
    mpz_class g;

    Schnorr();

    Schnorr(mpz_class p, mpz_class q, mpz_class g);

    class Signature {
    public:
        mpz_class r;
        mpz_class s;

        Signature(mpz_class r, mpz_class s);
    };

    class PrivateKey {
    public:
        mpz_class x;

        mpz_class p;
        mpz_class q;
        mpz_class g;

        PrivateKey(mpz_class x, mpz_class p, mpz_class q, mpz_class g);

        Signature sign(const string &m_str) const;
    };

    PrivateKey getPrivateKey() const;

    PrivateKey getPrivateKey(const mpz_class &x) const;

    class PublicKey {
    public:
        mpz_class y;

        mpz_class p;
        mpz_class q;
        mpz_class g;

        PublicKey(mpz_class y, mpz_class p, mpz_class q, mpz_class g);

        bool verify(const Signature &sig, const string &m_str) const;
    };

    PublicKey getPublicKey(const PrivateKey &sk) const;
};

ostream &operator<<(ostream &os, const Schnorr &param);

ostream &operator<<(ostream &os, const Schnorr::Signature &sig);

ostream &operator<<(ostream &os, const Schnorr::PrivateKey &sk);

ostream &operator<<(ostream &os, const Schnorr::PublicKey &pk);

#endif //SCHNORR_H
