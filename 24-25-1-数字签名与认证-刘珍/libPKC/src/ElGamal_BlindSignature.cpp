//
// Created by 张笑宇 on 2024/11/4.
//

#include "ElGamal_BlindSignature.h"
#include "Tools.h"

void ElGamal_BlindSignature::elgamal_blind_signature() {
    mpz_class p("0xb9c0faef108e0da9bc6a7fd87b9b837a19c2fa3b2daa3ef276fd87fcb7fe690f");
    mpz_class q("0x169f3edc9665f26b65fc6e805e3c997160a388a376e9266cfdbb512e107");
    mpz_class g("0x5119a79a9849f5c98659566b890077dfa71a7cb22f92e9089d3462b3b2bc16fa");
    mpz_class x = getRandNum(1, p - 1);
    mpz_class y = powm(g, x, p);

    // Message
    mpz_class m = 11;

    // Signer
    mpz_class k_prime = getRandNumWithCoprime(1, q ,q);
    mpz_class w_prime = powm(g, k_prime, p); // Send to user

    // User
    mpz_class alpha = getRandNumWithCoprime(1, q ,q);
    mpz_class beta = getRandNumWithCoprime(1, q ,q);
    mpz_class r = powm(g, alpha, p) * powm(w_prime, beta, p) % p;
    mpz_class M = beta * m * w_prime * getInv(r, q) % q;

    // Signer
    mpz_class s_prime = (x * w_prime + k_prime * M) % q;

    // User
    mpz_class s = (s_prime * r * getInv(w_prime, q) + alpha * m) % q;
    // Verify
    if (powm(y, r, p) * powm(r, m, p) % p == powm(g, s, p))
        cout << "验证通过" << endl;

}
