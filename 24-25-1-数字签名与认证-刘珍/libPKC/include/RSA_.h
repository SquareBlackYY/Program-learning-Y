//
// Created by 张笑宇 on 2024/10/2.
//

#ifndef RSA_H
#define RSA_H

#include "Tools.h"

class RSA_ {
    public:
    mpz_class p;
    mpz_class q;
    mpz_class n;

    RSA_();

    RSA_(mpz_class p, mpz_class q, mpz_class n);

    class PublicKey {
    public:
        mpz_class e;

        mpz_class n;

        PublicKey(mpz_class e, mpz_class n);

        mpz_class encrypt(const string &m_str) const;

        string verify(const mpz_class &sig) const;
        bool verifyHash(const mpz_class &sig, const string &m_str) const;
    };

    PublicKey getPublicKey() const;
    PublicKey setPublicKey(const mpz_class &e) const;

    class PrivateKey {
        public:
        mpz_class d;

        mpz_class p;
        mpz_class q;
        mpz_class n;

        PrivateKey(mpz_class d, mpz_class p, mpz_class q, mpz_class n);

        string decrypt(const mpz_class &c) const;
        string decryptWithCRT(const mpz_class &c) const;

        mpz_class sign(const string &m_str) const;
        mpz_class signHash(const string &m_str) const;
    };

    PrivateKey getPrivateKey(const PublicKey &pk) const;
    PrivateKey setPrivateKey(const mpz_class &d) const;
};

ostream &operator<<(ostream &os, const RSA_ &param);

ostream &operator<<(ostream &os, const RSA_::PublicKey &pk);

ostream &operator<<(ostream &os, const RSA_::PrivateKey &sk);

#endif //RSA_H
