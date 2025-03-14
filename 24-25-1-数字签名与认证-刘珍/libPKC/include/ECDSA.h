//
// Created by 张笑宇 on 2024/9/29.
//

#ifndef ECDSA_H
#define ECDSA_H

#include "EllipticCurve.h"
#include "ECPoint.h"
#include "Tools.h"

/**
 * @class ECDSA
 * @brief 实现ECDSA算法签名验签
 */
class ECDSA {
public:
    EllipticCurve E; ///< 椭圆曲线
    ECPoint G;
    mpz_class n;

    ECDSA();

    ECDSA(EllipticCurve E, ECPoint G, mpz_class n);

    class Signature {
    public:
        mpz_class r;
        mpz_class s;

        Signature(mpz_class r, mpz_class s);
    };

    class PrivateKey {
    public:
        mpz_class d;

        ECPoint G;
        mpz_class n;

        PrivateKey(mpz_class d, ECPoint G, mpz_class n);

        Signature sign(const string &m_str) const;
    };

    PrivateKey getPrivateKey() const;

    class PublicKey {
    public:
        ECPoint Q;

        ECPoint G;
        mpz_class n;

        PublicKey(ECPoint Q, ECPoint G, mpz_class n);

        bool verify(const Signature &sig, const string &m_str) const;
    };

    PublicKey getPublicKey(const PrivateKey &sk) const;
};

ostream &operator<<(ostream &os, const ECDSA &param);

ostream &operator<<(ostream &os, const ECDSA::Signature &sig);

ostream &operator<<(ostream &os, const ECDSA::PrivateKey &sk);

ostream &operator<<(ostream &os, const ECDSA::PublicKey &pk);

#endif //ECDSA_H
