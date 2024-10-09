#include "pkc.h"

int main()
{
    EllipticCurve E(23, 1, 4);
    ECPoint G(E, 0, 2);
    mpz_class n(29);

    ECDSA param(E, G, n);
    cout << param;

    ECDSA::PrivateKey sk = param.getPrivateKey();
    cout << sk;
    
    ECDSA::PublicKey pk = param.getPublicKey(sk);
    cout << pk;

    string m = "This is a test message for ECDSA signature";

    ECDSA::Signature sig = sk.sign(m);
    cout << sig;
    
    cout << (pk.verify(sig, m) ? "签名合法" : "签名非法") << endl;

    return 0;
}