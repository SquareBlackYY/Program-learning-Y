#include "pkc.h"

int main()
{
    mpz_class p("0xaa5ae5a2ff388b78174378cfdea0f7363893a63c68227df45ed8be2cde31241f");
    mpz_class g("0x5");
    mpz_class x("0x4A5B6C7D8E9F0A1B2C3D4E5F6A7B8C9");

    ElGamal param(p, g);
    cout << param;

    ElGamal::PrivateKey sk = param.getPrivateKey(x);
    cout << sk;
    
    ElGamal::PublicKey pk = param.getPublicKey(sk);
    cout << pk;

    string m = "This is a test message for Elgamal signature";

    ElGamal::Signature sig = sk.sign(m);
    cout << sig;
    
    cout << (pk.verify(sig, m) ? "签名合法" : "签名非法") << endl;

    cout << "随机数重用攻击" << endl;

    string m_2 = "This is a test message for ElGamal random nunber reuse attack";

    cout << (sk.signReuseRandNum(m, m_2) ? "攻击成功" : "攻击失败") << endl;

    return 0;
}