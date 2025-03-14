//
// Created by 张笑宇 on 2024/11/10.
//

#include "Shamir_IBS.h"

#include <gmpxx.h>

#include "Tools.h"

void Shamir_IBS::shamir_ibs() {
    // 系统参数建立
    mpz_class p("8384770691199731090227901761369715679766458105128220592036873364412961378312508844718993092171307186077124445798049353269284692982195512524426286648356027");
    mpz_class q("12181018780270384245980882487763195755475121960663023641593208090932207734556605764818243649182313018177847426957507816306095263109288627131434655751222979");
    mpz_class n = p * q;
    mpz_class phi_n = (p - 1) * (q - 1);
    mpz_class e = 5;
    mpz_class d = getInv(e, phi_n);

    // 用户私钥生成
    mpz_class ID = 12;
    mpz_class g = powm(ID, d, n);

    // 签名
    mpz_class m = 11;
    mpz_class r = getRandNumWithCoprime(1, n, n);
    mpz_class t = powm(r, e, n);
    mpz_class s = g * powm(r, sha256(t + m), n) % n;

    // 验证签名
    if (powm(s, e, n) == ID * powm(t, sha256(t + m), n) % n)
        cout << "验证通过" << endl;
}
