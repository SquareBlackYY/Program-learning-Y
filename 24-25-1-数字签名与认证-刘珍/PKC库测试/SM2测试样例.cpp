#include "pkc.h"

int main()
{
    EllipticCurve E(23, 1, 1);
    ECPoint G(E, 3, 10);

    SM2 param(E, G);
    cout << param;

    SM2::PrivateKey sk = param.getPrivateKey();
    cout << sk;
    
    SM2::PublicKey pk = param.getPublicKey(sk);
    cout << pk;

    mpz_class e = 3;

    SM2::Signature sig = sk.sign(e);
    cout << sig;
    
    cout << (pk.verify(sig, e) ? "签名合法" : "签名非法") << endl;

    return 0;
}