#include "pkc.h"

int main()
{
    ElGamal param;
    cout << param;

    ElGamal::PrivateKey sk = param.getPrivateKey(mpz_class("0x4A5B6C7D8E9F0A1B2C3D4E5F6A7B8C9"));
    cout << sk;
    
    ElGamal::PublicKey pk = param.getPublicKey(sk);
    cout << pk;

    string m = "This is a test message for Elgamal signature";

    ElGamal::Signature sig = sk.sign(m);
    cout << sig;
    
    cout << (pk.verify(sig, m) ? "签名合法" : "签名非法") << endl;

    return 0;
}