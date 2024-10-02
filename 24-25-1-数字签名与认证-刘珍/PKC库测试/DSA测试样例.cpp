#include "pkc.h"

int main()
{
    DSA_ param;
    cout << param;

    DSA_::PrivateKey sk = param.getPrivateKey(mpz_class("0x3B2F0C9E3A1B5D8A6E7C0D4F8A6B2E1C3D9F5E1"));
    cout << sk;
    
    DSA_::PublicKey pk = param.getPublicKey(sk);
    cout << pk;

    string m = "This is a test message for DSA signature";

    DSA_::Signature sig = sk.sign(m);
    cout << sig;
    
    cout << (pk.verify(sig, m) ? "签名合法" : "签名非法") << endl;

    return 0;
}