#include "pkc.h"

int main()
{
    mpz_class p("0xa030b2bbea795e7533769ff4e6bed8becae8e1f57d80062ed2b38397cc4c110f");
    mpz_class q("0x71e886bc4600d3869118146a5abf785911d");
    mpz_class g("0x12972b7570fb64952411d8a190995caaf1a573f5141c26b6bb17380a1880d00d");
    mpz_class x("0x3B2F0C9E3A1B5D8A6E7C0D4F8A6B2E1C3D9F5E1");

    DSA_ param(p, q, g);
    cout << param;

    DSA_::PrivateKey sk = param.getPrivateKey(x);
    cout << sk;
    
    DSA_::PublicKey pk = param.getPublicKey(sk);
    cout << pk;

    string m = "This is a test message for DSA signature";

    DSA_::Signature sig = sk.sign(m);
    cout << sig;
    
    cout << (pk.verify(sig, m) ? "签名合法" : "签名非法") << endl;

    return 0;
}