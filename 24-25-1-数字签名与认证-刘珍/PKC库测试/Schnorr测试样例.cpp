#include "pkc.h"

int main()
{
    Schnorr param;
    cout << param;

    Schnorr::PrivateKey sk = param.getPrivateKey(mpz_class("0x7C5D9F8B4A2F30D2E5AFA59F3C7B9A18E0DDAF23"));
    cout << sk;
    
    Schnorr::PublicKey pk = param.getPublicKey(sk);
    cout << pk;

    string m = "This is a test message for Schnorr signature";

    Schnorr::Signature sig = sk.sign(m);
    cout << sig;
    
    cout << (pk.verify(sig, m) ? "签名合法" : "签名非法") << endl;

    return 0;
}