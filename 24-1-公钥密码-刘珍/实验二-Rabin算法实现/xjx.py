import random
import sympy
#e取0
# 生成随机素数
def choose_p_q():
    while True:
        prime = random.randint(2**12, 2**16)
        if (sympy.isprime(prime) and (prime%4 == 3)):
            return prime

#求逆
def mod_inv(a, m):
    m0, x0, x1 = m, 0, 1
    while a > 1:
        q = a // m
        m, a = a % m, m
        x0, x1 = x1 - q * x0, x0
    return x1 + m0 if x1 < 0 else x1

# Rabin加密
def rsa_encrypt(m, n):
    return pow(m, 2, n)

# Rabin解密crt
def crt_decrypt(c, p, q):
    n = p * q
    n1 = n // p
    n2 = n // q
    inv_p = mod_inv(n1, p)
    inv_q = mod_inv(n2, q)
    x1 = c % p
    x2 = c % q
    x = []
    x.append((x1 * n1 * inv_p + x2 * n2 * inv_q) % n)
    x.append((-x1 * n1 * inv_p + x2 * n2 * inv_q) % n)
    x.append((x1 * n1 * inv_p - x2 * n2 * inv_q) % n)
    x.append((-x1 * n1 * inv_p - x2 * n2 * inv_q) % n)

    return x

def main():
    '''
    p = choose_p_q()
    q = choose_p_q()
    while p == q:
        q = choose_p_q()'''
    p,q = 7,11
    n = p*q
    m = 17
    encrypted_message = rsa_encrypt(m,n)
    plain_text = crt_decrypt(encrypted_message,p,q)
    print("明文：", plain_text)
    print("密文：", encrypted_message)

main()