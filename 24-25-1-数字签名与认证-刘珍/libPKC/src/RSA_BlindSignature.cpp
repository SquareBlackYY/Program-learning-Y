//
// Created by 张笑宇 on 2024/11/4.
//

#include "RSA_BlindSignature.h"
#include "Tools.h"

void RSA_BlindSignature::rsa_blind_signature() {
    // 参数生成
    mpz_class p = 97, q = 89;
    mpz_class n = p * q, phi_n = (p - 1) * (q - 1);
    mpz_class e = 5, d = getInv(e, phi_n);

    // 用户消息
    mpz_class m = 10;
    cout << "用户消息: " << m << endl;

    // 用户盲化
    mpz_class r = getRandNumWithCoprime(1, n, n);
    mpz_class m_prime = powm(r, e, n) * sha256(m) % n; // 发送给签名者
    cout << "盲化结果: " << m_prime << endl;

    // 签名者签名
    mpz_class sigma_prime = powm(m_prime, d, n); // 发送给用户
    cout << "签名结果: " << sigma_prime << endl;

    // 用户去盲化
    mpz_class sigma = sigma_prime * getInv(r, n) % n;
    cout << "去盲化结果: " << sigma << endl;
    // 验证
    if (powm(sigma, e, n) == sha256(m) % n)
        cout << "验证通过" << endl;
}
