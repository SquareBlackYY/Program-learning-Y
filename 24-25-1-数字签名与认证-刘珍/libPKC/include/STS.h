//
// Created by 张笑宇 on 2024/10/14.
//

#ifndef STS_H
#define STS_H

#include <gmpxx.h>

#include "ElGamal.h"
#include "Session.h"

class STS final : public Session {
    std::string targetID;
    int step = -1;

    mpz_class g;
    mpz_class p;
    mpz_class randNum;
    mpz_class received_g_randNum_mod_p;
    mpz_class sharedKey;

    ElGamal elgamal = ElGamal(mpz_class("0xaa5ae5a2ff388b78174378cfdea0f7363893a63c68227df45ed8be2cde31241f"), mpz_class("0x5"));
    ElGamal::PrivateKey elgamalPrivateKey;
    ElGamal::PublicKey elgamalPublicKey;

    ElGamal::PublicKey targetElGamalPublicKey;

public:
    explicit STS(std::string id);

    Message executeStep(const Message &msg, Host *host) override;
};


#endif //STS_H
