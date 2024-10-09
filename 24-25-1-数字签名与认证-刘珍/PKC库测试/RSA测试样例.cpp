#include "pkc.h"

int main()
{
    mpz_class p("8384770691199731090227901761369715679766458105128220592036873364412961378312508844718993092171307186077124445798049353269284692982195512524426286648356027");
    mpz_class q("12181018780270384245980882487763195755475121960663023641593208090932207734556605764818243649182313018177847426957507816306095263109288627131434655751222979");
    mpz_class n("102135049257764615042078588005181805961847617924788520924614970166905740850252377792164156899761196828021256396735835844272953844045954140171199203152941953733292753140561165735333672566395754320988951863125115229868139990823128135019358359614902902066874352974805819176036025088817935594035047801625055544433");
    mpz_class e("65537");
    mpz_class d("66473417932476187344627006008407842157813880631764187733929336672401810736327035450920395324815204991110650019780739766873957814119293344139376700356830267136528754388684199957664206587604561484552338778247510929052310097742136001829518535632056741801490247649680438463755276345908312841043655459326456925849");

    RSA_ param(p, q, n);
    cout << param;

    RSA_::PublicKey pk = param.setPublicKey(e);
    cout << pk;

    RSA_::PrivateKey sk = param.setPrivateKey(d);
    cout << sk;

    string m = "This is a test message for RSA test";

    mpz_class c = pk.encrypt(m);
    cout << "密文 c : " << c << endl;

    cout << "解密结果: " << sk.decrypt(c) << endl;
    cout << "快速解密结果: " << sk.decryptWithCRT(c) << endl;

    mpz_class sig = sk.sign(m);
    cout << "签名值: " << sig << endl;

    cout << "签名还原结果: " << pk.verify(sig) << endl;

    mpz_class sig_hash = sk.signHash(m);
    cout << "对哈希值的签名值: " << sig_hash << endl;

    cout << "哈希后签名验签结果: " << (pk.verifyHash(sig_hash, m) ? "签名合法" : "签名非法") << endl;

    return 0;
}