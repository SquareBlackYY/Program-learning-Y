#include <iostream>
#include <gmp.h>
using namespace std;

#define length 1

void ExEuclid(mpz_t, mpz_t, mpz_t);
void RSA_encrypt(mpz_t[], mpz_t[], mpz_t, mpz_t);
void RSA_decrypt(mpz_t[], mpz_t, mpz_t, mpz_t, mpz_t);

int main()
{
    mpz_t p, q, n, fn, e, d, p_1, q_1;
    mpz_inits(p, q, n, fn, e, d, p_1, q_1, NULL);

    mpz_set_ui(p, 137);
    mpz_set_ui(q, 131);

    mpz_mul(n, p, q);

    mpz_sub_ui(p_1, p, 1);
    mpz_sub_ui(q_1, q, 1);
    mpz_mul(fn, p_1, q_1);

    mpz_set_ui(e, 3);

    // 辗转相除法求e的逆
    ExEuclid(d, fn, e);

    mpz_t m[length], c[length];
    for (int i = 0; i < length; i++)
    {
        mpz_init(m[i]);
        mpz_init(c[i]);
    }

    mpz_set_ui(m[0], 513);

    // 加密
    RSA_encrypt(m, c, e, n);

    // 输出加密结果
    for (int i = 0; i < length; i++)
        gmp_printf("%Zd ", c[i]);
    cout << endl;

    // 解密
    RSA_decrypt(c, d, e, p, q);

    for (int i = 0; i < length; i++)
    {
        mpz_clear(m[i]);
        mpz_clear(c[i]);
    }
    mpz_clears(p, q, n, fn, e, d, NULL);

    return 0;
}

void ExEuclid(mpz_t result, mpz_t f, mpz_t d)
{
    mpz_t x, y, t, q;
    mpz_inits(x, y, t, q, NULL);

    mpz_set_ui(x, 1);
    mpz_set_ui(y, 0);
    mpz_set(t, f);

    while (1)
    {
        mpz_fdiv_q(q, x, y);
        mpz_submul(t, q, y);
        mpz_set(x, y);
        mpz_set(y, t);

        if (mpz_cmp_ui(y, 1) == 0)
        {
            mpz_mod(result, y, f);
            if (mpz_sgn(result) < 0)
            {
                mpz_add(result, result, f);
            }
            break;
        }
        else if (mpz_cmp_ui(y, 0) == 0)
        {
            mpz_set_ui(result, 0);
            break;
        }
    }

    mpz_clears(x, y, t, q, NULL);
}

void RSA_encrypt(mpz_t m[], mpz_t c[], mpz_t e, mpz_t n)
{
    for (int i = 0; i < length; i++)
        mpz_powm(c[i], m[i], e, n);
}

void RSA_decrypt(mpz_t c[], mpz_t d, mpz_t e, mpz_t p, mpz_t q)
{
    mpz_t m[length], q_inv, dp, dq, m1, m2, h;
    mpz_inits(q_inv, dp, dq, m1, m2, h, NULL);

    ExEuclid(q_inv, p, q);

    mpz_sub_ui(dp, p, 1);
    mpz_sub_ui(dq, q, 1);

    ExEuclid(dp, dp, e);
    ExEuclid(dq, dq, e);

    for (int i = 0; i < length; i++)
    {
        mpz_init(m[i]);

        mpz_powm(m1, c[i], dp, p);
        mpz_powm(m2, c[i], dq, q);

        mpz_sub(h, m1, m2);
        mpz_abs(h, h);
        mpz_mul(h, h, q_inv);
        mpz_mod(h, h, p);

        mpz_mul(h, h, q);
        mpz_add(m[i], m2, h);
    }

    for (int i = 0; i < length; i++)
        gmp_printf("%Zd ", m[i]);
    cout << endl;

    mpz_clears(q_inv, dp, dq, m1, m2, h, NULL);
}