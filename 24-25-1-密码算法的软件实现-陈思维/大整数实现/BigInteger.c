#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#define MAX_SIZE 32 // 大整数的最大块长度 (2^32进制)
#define NUM_PRIMALITY_TEST 5 // 素性检测的测试次数

// 大整数结构体
typedef struct {
    uint32_t value[MAX_SIZE]; // 数值部分 (2^32进制)
    bool isNegative; // 是否为负数
    int length; // 有效块长度 (2^32进制)
} BigInteger;

static const BigInteger ZERO = {{0}, false, 0};
static const BigInteger ONE = {{1}, false, 1};
static const BigInteger TWO = {{2}, false, 1};

void Print(const char *str, const BigInteger *num);

static void update_length(BigInteger *num);

BigInteger GetRandomNum(int len, bool mode);

static int compare(const uint32_t *x, const uint32_t *y, int max_len);

static void add(uint32_t *x, const uint32_t *y, int times);

static void subtract(uint32_t *x, const uint32_t *y, int times);

int Compare(BigInteger x, BigInteger y);

BigInteger RightShift(BigInteger num, int n_bit);

BigInteger Add(BigInteger x, BigInteger y);

BigInteger Subtract(BigInteger x, BigInteger y);

BigInteger Multiply(BigInteger x, BigInteger y);

void DivideAndMod(BigInteger *quot, BigInteger *rem, const BigInteger *x, const BigInteger *y);

BigInteger Mod(BigInteger x, BigInteger y);

static void ex_Euclid(BigInteger *res, const BigInteger *a, const BigInteger *b);

bool isCoprime(const BigInteger *a, const BigInteger *b);

BigInteger GetRandomNumWithCoprime(const BigInteger *coprime, int len);

BigInteger ModInverse(BigInteger num, BigInteger mod);

BigInteger ModExp(BigInteger base, BigInteger exp, BigInteger mod);

bool isPrime(const BigInteger *num);

BigInteger GetRandomPrime(int len);

void RSAKeygen(BigInteger *p, BigInteger *q, BigInteger *n, BigInteger *e, BigInteger *d);

BigInteger RSAEncrypt(const BigInteger *m, const BigInteger *e, const BigInteger *n);

BigInteger RSADecrypt(const BigInteger *c, const BigInteger *d, const BigInteger *n);


int main() {
    srand(time(NULL));

    // RSA参数生成
    BigInteger p, q, n, e, d;
    RSAKeygen(&p, &q, &n, &e, &d);
    Print("大整数 p = ", &p);
    Print("大整数 q = ", &q);
    Print("模数 n = ", &n);
    Print("公钥 e = ", &e);
    Print("私钥 d = ", &d);

    // 256 (32 * 8) 位明文 m
    BigInteger m = GetRandomNum(8, false);
    Print("明文 m = ", &m);

    // 加密
    BigInteger c = RSAEncrypt(&m, &e, &n);
    Print("加密 c = ", &c);

    // 解密
    BigInteger m_decrypted = RSADecrypt(&c, &d, &n);
    Print("解密 m = ", &m_decrypted);

    if (Compare(m, m_decrypted) == 0)
        printf("RSA加解密验证成功!\n");

    return 0;
}

// 打印大整数 (16进制)
void Print(const char *str, const BigInteger *num) {
    printf("%s", str);

    if (num->length == 0) {
        printf("0x0\n");
        return;
    }

    if (num->isNegative)
        printf("-");

    printf("0x");

    printf("%x ", num->value[num->length - 1]);
    for (int i = num->length - 2; i >= 0; i--)
        printf("%08x ", num->value[i]);
    printf("\n");
}

// 更新有效长度 (辅助函数)
static void update_length(BigInteger *num) {
    for (int i = MAX_SIZE - 1; i >= 0; i--)
        if (num->value[i] != 0) {
            num->length = i + 1;
            return;
        }

    // 0 判断
    num->isNegative = false;
    num->length = 0;
}

// 生成随机数 (指定块长度len) (false 生成定长的随机数, true 生成范围内的随机数)
BigInteger GetRandomNum(const int len, const bool mode) {
    BigInteger num = ZERO;
    for (size_t i = 0; i < len; i++)
        num.value[i] = (uint32_t) rand() | ((uint32_t) (rand() & 1) << 31);
    if (!mode)
        num.value[len - 1] |= 0x80000000; // 最高位为 1
    update_length(&num);
    return num;
}

// 按块比较器 (1 表示 x > y, -1 表示 x < y, 0 表示 x = y) (辅助函数)
static int compare(const uint32_t *x, const uint32_t *y, const int max_len) {
    // 按位比较
    for (int i = max_len - 1; i >= 0; i--)
        if (x[i] != y[i])
            return x[i] > y[i] ? 1 : -1;
    return 0;
}

// 按块加法器 (x += y) (辅助函数)
static void add(uint32_t *x, const uint32_t *y, const int times) {
    for (uint32_t i = 0, carry = 0; i < times || carry; i++) {
        const uint64_t sum = (uint64_t) x[i] + y[i] + carry;

        // 进位处理
        carry = sum >> 32;

        x[i] = sum & UINT32_MAX;
    }
}

// 按块减法器 (x -= y) (x 长度大于 y) (辅助函数)
static void subtract(uint32_t *x, const uint32_t *y, const int times) {
    for (uint32_t i = 0, borrow = 0; i < times || borrow; i++) {
        uint64_t diff = (uint64_t) x[i] - y[i] - borrow;

        // 借位处理
        if (diff > UINT32_MAX) {
            diff += 1ULL << 32;
            borrow = 1;
        } else
            borrow = 0;

        x[i] = diff & UINT32_MAX;
    }
}

// 大小比较 (1 表示 x > y, -1 表示 x < y, 0 表示 x = y)
int Compare(const BigInteger x, const BigInteger y) {
    // 符号比较
    if (x.isNegative && !y.isNegative)
        return -1;
    if (!x.isNegative && y.isNegative)
        return 1;

    const int max_len = x.length > y.length ? x.length : y.length;

    return compare(x.value, y.value, max_len) * (x.isNegative ? -1 : 1);
}

// 右移 (num >> n_bit) (负数为左移)
BigInteger RightShift(BigInteger num, const int n_bit) {
    // 0 判断
    if (num.length == 0)
        return ZERO;
    if (n_bit == 0)
        return num;

    const int word_num = n_bit / 32, bit_num = n_bit % 32;

    // 右移
    if (n_bit > 0) {
        if (word_num) {
            // 移动块
            memmove(num.value, num.value + (num.length - word_num), (num.length - word_num) * sizeof(uint32_t));

            // 高位块置 0
            memset(num.value + (num.length - word_num), 0, word_num * sizeof(uint32_t));
        }

        if (bit_num)
            for (int i = 0; i < num.length - word_num; i++)
                num.value[i] = num.value[i + 1] << (32 - bit_num) | num.value[i] >> bit_num;
    }
    // 左移
    else {
        if (word_num) {
            // 移动块
            memmove(num.value - word_num, num.value, num.length * sizeof(uint32_t));

            // 低位块置 0
            memset(num.value, 0, -word_num * sizeof(uint32_t));
        }

        if (bit_num) {
            for (int i = num.length - word_num; i > -word_num; i--)
                num.value[i] = num.value[i] << -bit_num | num.value[i - 1] >> (32 + bit_num);
            num.value[word_num] <<= -bit_num;
        }
    }

    // 更新有效长度
    update_length(&num);
    return num;
}

// 加法 (x + y)
BigInteger Add(const BigInteger x, const BigInteger y) {
    // 0 判断
    if (x.length == 0 && y.length == 0)
        return ZERO;
    if (x.length == 0)
        return y;
    if (y.length == 0)
        return x;

    BigInteger res;

    const int max_len = x.length > y.length ? x.length : y.length;
    const int min_len = x.length < y.length ? x.length : y.length;
    const int cmp = compare(x.value, y.value, max_len);

    // 符号相同
    if (x.isNegative == y.isNegative) {
        if (cmp >= 0) {
            res = x;
            add(res.value, y.value, min_len);
        } else {
            res = y;
            add(res.value, x.value, min_len);
        }
    }
    // 符号不同，转为减法
    else {
        // 数值部分相同，结果为 0
        if (cmp == 0)
            return ZERO;
        if (cmp > 0) {
            res = x;
            subtract(res.value, y.value, min_len);
        } else {
            res = y;
            subtract(res.value, x.value, min_len);
            res.isNegative = !res.isNegative;
        }
    }

    // 更新有效长度
    update_length(&res);
    return res;
}

// 减法 (x - y)
BigInteger Subtract(const BigInteger x, BigInteger y) {
    // 0 判断
    if (x.length == 0 && y.length == 0)
        return ZERO;
    if (x.length == 0) {
        y.isNegative = !y.isNegative;
        return y;
    }
    if (y.length == 0)
        return x;

    BigInteger res = ZERO;

    const int max_len = x.length > y.length ? x.length : y.length;
    const int min_len = x.length < y.length ? x.length : y.length;
    const int cmp = compare(x.value, y.value, max_len);

    // 符号相同
    if (x.isNegative == y.isNegative) {
        // 数值部分相同，结果为 0
        if (cmp == 0)
            return ZERO;
        if (cmp > 0) {
            res = x;
            subtract(res.value, y.value, min_len);
        } else {
            res = y;
            subtract(res.value, x.value, min_len);
            res.isNegative = !res.isNegative;
        }
    }
    // 符号不同，转为加法
    else {
        if (cmp >= 0) {
            res = x;
            add(res.value, y.value, min_len);
        } else {
            res = y;
            add(res.value, x.value, min_len);
            res.isNegative = !res.isNegative;
        }
    }

    // 更新有效长度
    update_length(&res);
    return res;
}

// 乘法 (x * y)
BigInteger Multiply(const BigInteger x, const BigInteger y) {
    // 0 判断
    if (x.length == 0 || y.length == 0)
        return ZERO;

    BigInteger res = ZERO;

    // 符号判断
    res.isNegative = x.isNegative ^ y.isNegative;

    // 按块乘法
    for (int i = 0; i < x.length; i++) {
        for (int j = 0; j < y.length; j++) {
            const uint64_t prod = (uint64_t) x.value[i] * y.value[j];
            const BigInteger tmp = {{prod & UINT32_MAX, prod >> 32}, false, 2};
            add(res.value + i + j, tmp.value, 2);
        }
    }

    // 更新有效长度
    update_length(&res);
    return res;
}

// 带余除法 (quot = x // y, rem = x % y) (rem不会被修正为正数) (性能较低)
void DivideAndMod(BigInteger *quot, BigInteger *rem, const BigInteger *x, const BigInteger *y) {
    // 0 判断
    if (y->length == 0) {
        printf("Error: Division by zero.\n");
        return;
    }
    if (x->length == 0)
        return;

    *quot = *rem = ZERO;

    const int max_len = x->length > y->length ? x->length : y->length;
    const int min_len = x->length < y->length ? x->length : y->length;

    // 绝对值判断
    const int cmp = compare(x->value, y->value, max_len);
    if (cmp == 0) {
        *quot = ONE;
        return;
    }
    if (cmp < 0) {
        if (x->isNegative) {
            *rem = *y;
            subtract(rem->value, x->value, min_len);
            rem->isNegative = false;
        } else
            *rem = *x;
    }
    // 按位除法
    else {
        // 符号判断
        quot->isNegative = x->isNegative ^ y->isNegative;

        *rem = *x;
        for (int n = x->length - y->length; n >= 0; n--) {
            uint32_t count = 0;
            BigInteger tmp = RightShift(*y, -32);
            for (int i = -31; i <= 0; i++) {
                tmp = RightShift(tmp, 1);
                if (compare(rem->value + n, tmp.value, min_len + 1) >= 0) {
                    subtract(rem->value + n, tmp.value, min_len + 1);
                    count |= 1UL << -i;
                }
            }
            quot->value[n] = count;
        }
    }

    // 更新有效长度
    update_length(quot);
    update_length(rem);
}

// 取模 (x % y) (结果会被修正为正数) (性能较低)
BigInteger Mod(const BigInteger x, const BigInteger y) {
    BigInteger res, quot, rem;
    DivideAndMod(&quot, &rem, &x, &y);

    if (rem.isNegative) {
        res = y;
        subtract(res.value, rem.value, y.length);
        res.isNegative = false;

        // 更新有效长度
        update_length(&res);
    } else
        res = rem;

    return res;
}

// 扩展欧几里得算法 (辅助函数)
void ex_Euclid(BigInteger *res, const BigInteger *a, const BigInteger *b) {
    BigInteger x[3] = {ONE, ZERO, Mod(*a, *b)}, y[3] = {ZERO, ONE, *b};
    int times = 0;
    while (y[2].length != 0) {
        BigInteger q, rem;
        DivideAndMod(&q, &rem, &x[2], &y[2]);
        const BigInteger t[3] = {Subtract(x[0], Multiply(q, y[0])), Subtract(x[1], Multiply(q, y[1])), rem};

        times++;
        memcpy(x, y, 3 * sizeof(BigInteger));
        memcpy(y, t, 3 * sizeof(BigInteger));
    }

    memcpy(res, x, 3 * sizeof(BigInteger));
}

// 互素判断
bool isCoprime(const BigInteger *a, const BigInteger *b) {
    // 扩展的欧几里得算法
    BigInteger res[3];
    ex_Euclid(res, a, b);

    return Compare(res[2], ONE) == 0;
}

// 生成与给定数互素的随机数 (指定块长度len)
BigInteger GetRandomNumWithCoprime(const BigInteger *coprime, const int len) {
    BigInteger res;
    do {
        res = GetRandomNum(len, false);
    } while (!isCoprime(&res, coprime));
    return res;
}

// 模逆 (num ^ -1 % mod) (扩展欧几里得算法)
BigInteger ModInverse(const BigInteger num, const BigInteger mod) {
    // 扩展的欧几里得算法
    BigInteger res[3];
    ex_Euclid(res, &num, &mod);

    if (Compare(res[2], ONE) == 0)
        return Mod(res[0], mod);

    return ZERO;
}

// 快速模幂 (base ^ exp % mod) (exp为负数时自动求逆)
BigInteger ModExp(BigInteger base, BigInteger exp, const BigInteger mod) {
    // 0 判断
    if (base.length == 0 && exp.length == 0) {
        printf("Error: Base and exponent are both zero.\n");
        return ZERO;
    }
    if (mod.length == 0) {
        printf("Error: Modulus is zero.\n");
        return ZERO;
    }
    if (base.length == 0)
        return ZERO;
    if (exp.length == 0)
        return ONE;

    BigInteger res = ONE;
    base = Mod(base, mod);

    // exp 为负数时 base 自动求逆
    if (exp.isNegative) {
        base = ModInverse(base, mod);
        exp.isNegative = false;
    }

    // 快速模平方算法
    while (exp.length != 0) {
        if ((exp.value[0] & 1) == 1)
            res = Mod(Multiply(res, base), mod);
        base = Mod(Multiply(base, base), mod);
        exp = RightShift(exp, 1);
    }

    return res;
}

// 素性检测 (Miller-Rabin算法) (指定测试次数times)
bool isPrime(const BigInteger *num) {
    // 小于等于2, 直接判断
    const int cmp = Compare(*num, TWO);
    if (cmp < 0)
        return false;
    if (cmp == 0)
        return true;

    // 大于2且为偶数, 直接判断
    if ((num->value[0] & 1) == 0)
        return false;

    // Miller-Rabin算法
    // 分解 num - 1 = 2 ^ s * d
    int s = 0;
    BigInteger d = Subtract(*num, ONE);
    while ((d.value[0] & 1) == 0) {
        s++;
        d = RightShift(d, 1);
    }

    // 测试
    for (int i = 0; i < NUM_PRIMALITY_TEST; i++) {
        const BigInteger a = GetRandomNum(num->length, true);
        BigInteger x = ModExp(a, d, *num);

        if (Compare(x, ONE) == 0 || Compare(x, Subtract(*num, ONE)) == 0)
            continue;

        // 标志循环是否被打断
        bool flag = false;
        for (int j = 0; j < s - 1; j++) {
            x = Mod(Multiply(x, x), *num);
            if (Compare(x, ONE) == 0)
                return false;
            if (Compare(x, Subtract(*num, ONE)) == 0) {
                flag = true;
                break;
            }
        }
        if (!flag)
            return false;
    }
    return true;
}

// 生成随机素数 (指定块长度len)
BigInteger GetRandomPrime(const int len) {
    BigInteger res;
    do {
        res = GetRandomNum(len, false);
    } while (!isPrime(&res));
    return res;
} 

// RSA 参数生成
void RSAKeygen(BigInteger *p, BigInteger *q, BigInteger *n, BigInteger *e, BigInteger *d) {
    // 256 (32 * 8) 位大素数 p, q
    *p = GetRandomPrime(8);
    *q = GetRandomPrime(8);

    // 512 位模数 n
    *n = Multiply(*p, *q);
    const BigInteger phi_n = Multiply(Subtract(*p, ONE), Subtract(*q, ONE));

    // 256 (32 * 8) 位公钥 e
    *e = GetRandomNumWithCoprime(&phi_n, 8);
    *d = ModInverse(*e, phi_n);
}

// RSA 加密
BigInteger RSAEncrypt(const BigInteger *m, const BigInteger *e, const BigInteger *n) {
    return ModExp(*m, *e, *n);
}

// RSA 解密
BigInteger RSADecrypt(const BigInteger *c, const BigInteger *d, const BigInteger *n) {
    return ModExp(*c, *d, *n);
}
