//
// Created by 张笑宇 on 2024/10/2.
//

#ifndef DSA_H
#define DSA_H

#include "Tools.h"

class DSA_ {
public:
    mpz_class p;
    mpz_class q;
    mpz_class g;

    DSA_();

    DSA_(mpz_class p, mpz_class q, mpz_class g);

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

ostream &operator<<(ostream &os, const DSA_ &param);

ostream &operator<<(ostream &os, const DSA_::Signature &sig);

ostream &operator<<(ostream &os, const DSA_::PrivateKey &sk);

ostream &operator<<(ostream &os, const DSA_::PublicKey &pk);

#endif //DSA_H
