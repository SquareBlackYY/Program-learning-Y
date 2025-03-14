//
// Created by 张笑宇 on 2024/11/4.
//

#include "Schnorr_BlindSignature.h"
#include "Tools.h"

void Schnorr_BlindSignature::schnorr_blind_signature() {
    mpz_class p("0xb9c0faef108e0da9bc6a7fd87b9b837a19c2fa3b2daa3ef276fd87fcb7fe690f");
    mpz_class q("0x169f3edc9665f26b65fc6e805e3c997160a388a376e9266cfdbb512e107");
    mpz_class g("0x5119a79a9849f5c98659566b890077dfa71a7cb22f92e9089d3462b3b2bc16fa");
    mpz_class x = getRandNum(1, q);
    mpz_class y = powm(g, x, p);

    // Message
    mpz_class m = 10;
    cout << "用户消息: " << m << endl;

    // Signer
    mpz_class k_prime = getRandNumWithCoprime(1, q ,q);
    mpz_class w_prime = powm(g, k_prime, p); // Send to user
    cout << "签名者发送参数 w': " << w_prime << endl;

    // User
    mpz_class alpha = getRandNumWithCoprime(1, q ,q);
    mpz_class beta = getRandNumWithCoprime(1, q ,q);
    mpz_class w = powm(g, alpha, p) * powm(y, beta, p) * w_prime % p;
    mpz_class e = sha256(w + m);
    mpz_class e_prime = (e + beta) % q; // Send to Signer
    cout << "用户发送参数 e': " << e_prime << endl;

    // Signer
    mpz_class s_prime = (k_prime + x * e_prime) % q; // Send to user
    cout << "签名者发送参数 s': " << s_prime << endl;

    // User
    mpz_class s = (s_prime + alpha) % q;
    cout << "用户计算 s: " << s << endl;

    // Verify
    mpz_class w_verify = powm(g, s, p) * powm(getInv(y, p), e, p) % p;
    if (e == sha256(w_verify + m))
        cout << "验证通过" << endl;
}