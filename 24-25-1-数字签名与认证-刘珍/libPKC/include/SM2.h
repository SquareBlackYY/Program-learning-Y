//
// Created by 张笑宇 on 2024/10/14.
//

#ifndef SM2_H
#define SM2_H

#include "ECPoint.h"
#include "EllipticCurve.h"

class SM2 {
    public:
        EllipticCurve E;
        ECPoint G;
        mpz_class n;

        SM2();

        SM2(EllipticCurve E, ECPoint G);

        SM2(EllipticCurve E, ECPoint G, mpz_class n);

        class Signature {
        public:
            mpz_class r;
            mpz_class s;

            Signature(mpz_class r, mpz_class s);
        };

        class PrivateKey {
        public:
            mpz_class d;

            EllipticCurve E;
            ECPoint G;
            mpz_class n;

            PrivateKey(mpz_class d, EllipticCurve E, ECPoint G, mpz_class n);

            Signature sign(const mpz_class &e) const;
        };

        PrivateKey getPrivateKey() const;

        class PublicKey {
        public:
            ECPoint P;

            EllipticCurve E;
            ECPoint G;
            mpz_class n;

            PublicKey(ECPoint P, EllipticCurve E, ECPoint G, mpz_class n);

            bool verify(const Signature &sig, const mpz_class &e) const;
        };

        PublicKey getPublicKey(const PrivateKey &sk) const;
    };

    ostream &operator<<(ostream &os, const SM2 &param);

    ostream &operator<<(ostream &os, const SM2::Signature &sig);

    ostream &operator<<(ostream &os, const SM2::PrivateKey &sk);

    ostream &operator<<(ostream &os, const SM2::PublicKey &pk);




#endif //SM2_H
