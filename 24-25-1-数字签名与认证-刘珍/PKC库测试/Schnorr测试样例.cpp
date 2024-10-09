#include "pkc.h"

int main()
{
    mpz_class p("0xb9c0faef108e0da9bc6a7fd87b9b837a19c2fa3b2daa3ef276fd87fcb7fe690f");
    mpz_class q("0x169f3edc9665f26b65fc6e805e3c997160a388a376e9266cfdbb512e107");
    mpz_class g("0x5119a79a9849f5c98659566b890077dfa71a7cb22f92e9089d3462b3b2bc16fa");
    mpz_class x("0x7C5D9F8B4A2F30D2E5AFA59F3C7B9A18E0DDAF23");

    Schnorr param(p, q, g);
    cout << param;

    Schnorr::PrivateKey sk = param.getPrivateKey(x);
    cout << sk;
    
    Schnorr::PublicKey pk = param.getPublicKey(sk);
    cout << pk;

    string m = "This is a test message for Schnorr signature";

    Schnorr::Signature sig = sk.sign(m);
    cout << sig;
    
    cout << (pk.verify(sig, m) ? "签名合法" : "签名非法") << endl;

    return 0;
}