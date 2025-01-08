#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_SIZE 120

// 定义结构体表示大整数
typedef struct {
    uint32_t value[MAX_SIZE]; // 数值部分
    bool isNegative;          // 是否为负数
} BigInt;

const BigInt R_256 = { {0,0,0,0, 0,0,0,0, 1} ,false};
const BigInt R_512 = { {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 1},false };

void initializeBigInt(BigInt* num);
void copyBigInt(const BigInt* src, BigInt* dest);
int compareBigInt(const BigInt* num1, const BigInt* num2);
void addBigInt(const BigInt* num1, const BigInt* num2, BigInt* result);
void subtractBigInt(const BigInt* num1, const BigInt* num2, BigInt* result);
void multiplyBigInt(const BigInt* num1, const BigInt* num2, BigInt* result);
void divideBigInt(const BigInt* num1, const BigInt* num2, BigInt* result, BigInt* remainder);
void gcdExtendedBigInt(const BigInt* a, const BigInt* b, BigInt* x, BigInt* y, BigInt* gcd);
void modInverseBigInt(const BigInt* a, const BigInt* mod, BigInt* inverse);
void generateRSAKeys(BigInt* n, BigInt* e, BigInt* d);
void encryptRSA(const BigInt* message, const BigInt* n, const BigInt* e, BigInt* ciphertext);
void decryptRSA(const BigInt* ciphertext, const BigInt* n, const BigInt* d, BigInt* message);
void modExponentiation(const BigInt* base, const BigInt* exponent, const BigInt* mod, BigInt* result);
void modBigInt(const BigInt* num, const BigInt* mod, BigInt* result);
int clz(unsigned int x);
void bigIntRightShift(BigInt* bigInt);
void montgomeryMultiply(const BigInt* a, const BigInt* b, const BigInt* mod ,BigInt* modinv,BigInt* result,int RK);
BigInt montgomeryReduce(BigInt* x, const BigInt* mod, BigInt* R, int Rk);
void montgomeryInverse(const BigInt* a, const BigInt* mod, BigInt* inverse);
void montgomeryExponentiation(const BigInt* base, const BigInt* exponent, const BigInt* mod, BigInt* result, BigInt* R);
int montgomeryMillerRabinTest(const BigInt* n, int k);
void get_prime(uint8_t a, BigInt* n);
void get_256bits_num(uint8_t a, BigInt* n);
BigInt mod_2_k(BigInt a, int k);

int clz(unsigned int x) {
    int count = 0;
    if (x == 0) return 32; // 如果是0，返回32个零
    while ((x & 0x80000000) == 0) {
        count++;
        x <<= 1;
    }
    return count;
}

// 大整数右移函数
void bigIntRightShift(BigInt* bigInt) {
    int len1 = MAX_SIZE;
    BigInt temp;
	initializeBigInt(&temp);
    
    // 计算 bigInt 的长度
    while (bigInt->value[len1 - 1] == 0) len1--;
    
    // 右移操作
	for (int j = 0; j < len1; j++) {
	    temp.value[j] = ((bigInt->value[j + 1] & 1)<<31) | (bigInt->value[j] >> 1);
	}
	temp.value[len1] = bigInt->value[len1] >> 1;
	copyBigInt(&temp,bigInt);    
}

//mod2^k
BigInt mod_2_k(BigInt a, int k) {
    BigInt result = a;
    int len1 = MAX_SIZE;
    // 计算 num1 和 num2 的长度
    while (a.value[len1 - 1] == 0) len1--;

    // 计算需要保留的位数
    int full_words = k / 32;      // 完整的 32 位块
    int remaining_bits = k % 32; // 剩余的位数

    // 保留完整的块
    for (int i = full_words + 1; i < len1; ++i) {
        result.value[i] = 0; // 清零超出位数的块
    }

    // 保留部分位数
    if (remaining_bits > 0 && full_words < len1) {
        uint32_t mask = (1U << remaining_bits) - 1; // 生成部分位的掩码
        result.value[full_words] &= mask;       // 应用掩码
    }
    return result;
}

//div 2^k
BigInt division_2_k(BigInt a, int k) {
    BigInt result = { 0 }; // 初始化结果
    result.isNegative = a.isNegative; // 保留符号

    // 如果 k 为 0，直接返回原数
    if (k == 0) {
        result = a;
        return result;
    }
    int len1 = MAX_SIZE, len2 = MAX_SIZE;
    // 计算 num1 和 num2 的长度
    while (a.value[len1 - 1] == 0) len1--;


    // 计算需要右移的完整 position 和剩余的位数
    int shift_positions = k / 32;  // 完整 position 的移动数
    int bit_shift = k % 32;        // 单个 position 内的位移数

    // 如果右移超过了所有位数，结果为 0
    if (shift_positions >= len1) {
      len2 = 0;
        return result;
    }

    // 更新结果的长度
    len2 = len1 - shift_positions;

    // 逐个处理剩余部分
    uint32_t carry = 0; // 用于跨 position 的进位
    for (int i = len1 - 1; i >= shift_positions; i--) {
        uint64_t current = ((uint64_t)carry << 32) | a.value[i]; // 当前 position 数据加上进位
        result.value[i - shift_positions] = (current >> bit_shift) & 0xFFFFFFFF; // 右移并截断高位
        carry = (uint32_t)(current & ((1U << bit_shift) - 1)); // 保留未移出的部分作为下一个 position 的进位
    }

    // 移除结果中可能多余的高位 0
    while (len2 > 0 && result.value[len2 - 1] == 0) {
       len2--;
    }

    return result;
}


// 初始化大整数
void initializeBigInt(BigInt* num) {
    memset(num->value, 0, sizeof(num->value));
    num->isNegative = false;
}

// 拷贝大整数
void copyBigInt(const BigInt* src, BigInt* dest) {
    if (src == NULL || dest == NULL) {
        printf("Error: NULL pointer in copyBigInt\n");
        return;  // 如果源或者目标为空指针，报错
    }

    // 拷贝数值数组
    for (int i = 0; i < MAX_SIZE; i++) {
        dest->value[i] = src->value[i];
    }

    // 拷贝符号
    dest->isNegative = src->isNegative;
}


int compareBigInt(const BigInt* num1, const BigInt* num2) {
    // 处理符号比较
    if (num1->isNegative && !num2->isNegative) {
        return -1; // 负数小于正数
    }
    else if (!num1->isNegative && num2->isNegative) {
        return 1;  // 正数大于负数
    }

    bool bothNegative = num1->isNegative && num2->isNegative;

    // 计算 num1 和 num2 的有效长度
    int len1 = MAX_SIZE, len2 = MAX_SIZE;
    while (len1 > 0 && num1->value[len1 - 1] == 0) len1--;
    while (len2 > 0 && num2->value[len2 - 1] == 0) len2--;

    // 如果长度不同，长度较长的数值更大
    if (len1 != len2) {
        return (len1 > len2 ? 1 : -1) * (bothNegative ? -1 : 1);
    }

    // 数字相同长度时按位比较
    for (int i = len1 - 1; i >= 0; i--) {
        if (num1->value[i] != num2->value[i]) {
            return (num1->value[i] > num2->value[i] ? 1 : -1) * (bothNegative ? -1 : 1);
        }
    }

    // 如果所有位都相等，返回 0
    return 0;
}

// 大整数加法
void addBigInt(const BigInt* num1, const BigInt* num2, BigInt* result) {
    if (num1->isNegative == num2->isNegative) {
        result->isNegative = num1->isNegative;

        int len1 = MAX_SIZE, len2 = MAX_SIZE;
        // 计算 num1 和 num2 的长度
        while (num1->value[len1 - 1] == 0) len1--;
        while (num2->value[len2 - 1] == 0) len2--;

        int carry = 0;
        int maxLen = len1 > len2 ? len1 : len2;
        int i = 0;

        // 按位加法
        while (i < maxLen || carry) {
            uint32_t digit1 = (i < len1) ? num1->value[i] : 0;
            uint32_t digit2 = (i < len2) ? num2->value[i] : 0;

            uint64_t sum = digit1 + digit2 + carry;
            result->value[i] = sum & 0xFFFFFFFF;  // 结果保留到当前位
            carry = sum >> 32;  // 进位，超过32位时
            i++;
        }

        // 处理结果的长度
        result->value[i] = 0;
    }
    else {
        // 符号不同，转为减法
        BigInt absNum1 = *num1, absNum2 = *num2;
        absNum1.isNegative = absNum2.isNegative = false;

        if (compareBigInt(&absNum1, &absNum2) >= 0) {
            subtractBigInt(&absNum1, &absNum2, result);
            result->isNegative = num1->isNegative;
        }
        else {
            subtractBigInt(&absNum2, &absNum1, result);
            result->isNegative = num2->isNegative;
        }
    }
}

// 大整数减法
void subtractBigInt(const BigInt* num1, const BigInt* num2, BigInt* result) {
    // 如果两个数符号相同，进行普通减法
    if (num1->isNegative == num2->isNegative) {
        // 判断大小，较大的数减去较小的数
        if (compareBigInt(num1, num2) >= 0) {
            result->isNegative = num1->isNegative;
            int len1 = MAX_SIZE, len2 = MAX_SIZE;
            // 计算 num1 和 num2 的长度
            while (num1->value[len1 - 1] == 0 && len1 > 0) len1--;
            while (num2->value[len2 - 1] == 0 && len2 > 0) len2--;

            int borrow = 0;
            int maxLen = len1 > len2 ? len1 : len2;
            int i = 0;

            // 按位减法
            while (i < maxLen || borrow) {
                uint32_t digit1 = (i < len1) ? num1->value[i] : 0;
                uint32_t digit2 = (i < len2) ? num2->value[i] : 0;

                uint64_t diff = (uint64_t)(digit1) - (digit2 + borrow); // 计算差值

                if (diff > 0xffffffff) {
                    diff += ((uint64_t)1 << 32);  // 借位处理
                    borrow = 1;
                } else {
                    borrow = 0;
                }

                result->value[i] = (uint32_t)(diff & 0xFFFFFFFF);  // 只取低32位
                i++;
            }

            // 处理结果的长度
            if (i < MAX_SIZE) {
                result->value[i] = 0;  // 确保结果末尾为0
            }
        }
        else {
            // 如果 num1 小于 num2，反转计算顺序，结果取反
            subtractBigInt(num2, num1, result);
            result->isNegative = !num1->isNegative;  // 结果符号由 num2 决定
        }
    } else {
        // 两个数符号不同，转为加法
        BigInt absNum1 = *num1, absNum2 = *num2;
        absNum1.isNegative = absNum2.isNegative = false;

        addBigInt(&absNum1, &absNum2, result);
        result->isNegative = num1->isNegative;  // 结果符号取决于 num1
    }
}


//大整数乘法
void multiplyBigInt(const BigInt* num1, const BigInt* num2, BigInt* result) {
    int len1 = MAX_SIZE, len2 = MAX_SIZE;
    // 计算 num1 和 num2 的长度
    while (num1->value[len1 - 1] == 0) len1--;
    while (num2->value[len2 - 1] == 0) len2--;

    // 初始化结果
    memset(result->value, 0, sizeof(result->value));

    // 计算符号
    if (num1->isNegative != num2->isNegative) {
        result->isNegative = true;
    }
    else {
        result->isNegative = false;
    }

    // 执行乘法
    for (int i = 0; i < len1; i++) {
        for (int j = 0; j < len2; j++) {
            uint64_t prod = (uint64_t)num1->value[i] * (uint64_t)num2->value[j];

            // 当前位的低32位
            uint32_t low = (uint32_t)(prod & 0xFFFFFFFF);
            // 当前位的高32位
            uint32_t high = (uint32_t)(prod >> 32);

            // 处理进位
            uint64_t sum = (uint64_t)result->value[i + j] + low;
            result->value[i + j] = (uint32_t)(sum & 0xFFFFFFFF);
            uint32_t carry = (uint32_t)(sum >> 32);

            // 将高位加到下一个位置
            sum = (uint64_t)result->value[i + j + 1] + high + carry;
            result->value[i + j + 1] = (uint32_t)(sum & 0xFFFFFFFF);
        }
    }
}

// 大整数除法
void divideBigInt(const BigInt* num1, const BigInt* num2, BigInt* result, BigInt* remainder) {
    BigInt zero = {{0}, false};  // 用于判断除数是否为零
    BigInt dividend = *num1;     // 被除数
    BigInt divisor = *num2;      // 除数

    // 检查除数是否为0
    if (compareBigInt(num2, &zero) == 0) {
        printf("Error: Division by zero\n");
        return;
    }

    // 初始化结果和余数
    initializeBigInt(result);
    initializeBigInt(remainder);

    // 符号处理
    bool resultNegative = (num1->isNegative != num2->isNegative);
    dividend.isNegative = false; // 被除数符号处理为正
    divisor.isNegative = false;  // 除数符号处理为正

    // 计算除数 divisor 的有效位数
    int divisorBits = 0;
    for (int i = MAX_SIZE - 1; i >= 0; i--) {
        if (dividend.value[i] != 0) {
            divisorBits = (i + 1) * 32 - clz(dividend.value[i]);
            break;
        }
    }
	
    // 将余数初始化为0
    BigInt shiftedDivisor,temp;
    initializeBigInt(&shiftedDivisor);
	initializeBigInt(&temp);
	
    // 从最高位开始进行除法
    for (int i = divisorBits - 1; i >= 0; i--) {
        // 将余数左移一位，并加入当前位
        int wordIndex = i / 32;
        int bitIndex = i % 32;
        
		// 左移操作
		for (int j = MAX_SIZE - 1; j > 0; j--) {
		    temp.value[j] = (remainder->value[j] << 1) | (remainder->value[j - 1] >> 31);
		}
		temp.value[0] = (remainder->value[0] << 1) | ((dividend.value[wordIndex] >> bitIndex) & 1);
		//printf("余数%d：%08x %08x %08x %08x\n",i, temp.value[3], temp.value[2], temp.value[1], temp.value[0]);
		copyBigInt(&temp,remainder);
		
        // 如果余数大于等于除数，则减去除数并将商的当前位设置为1
        if(compareBigInt(remainder,&divisor) >= 0) {
			subtractBigInt(remainder, &divisor,remainder); // 余数更新
			//copyBigInt(&temp,remainder);			
            result->value[wordIndex] |= (1u << bitIndex);  // 结果更新
            //printf("除数%d：%08x %08x %08x %08x\n",i, result->value[3], result->value[2], result->value[1], result->value[0]);
            //printf("余数%d：%08x %08x %08x %08x\n",i, remainder->value[3], remainder->value[2], remainder->value[1], remainder->value[0]);
        }
        
    }

    // 结果符号修正
    result->isNegative = resultNegative;

    // 确保余数符号非负
    remainder->isNegative = false;
    if (compareBigInt(remainder, &zero) < 0) {
        // 余数为负时，将其修正
        addBigInt(remainder, &divisor, remainder);  // 余数加回除数
        printf("余数：%08x %08x %08x %08x\n",remainder->value[3], remainder->value[2], remainder->value[1], remainder->value[0]);
    }
}

void gcdExtendedBigInt(const BigInt* a, const BigInt* b, BigInt* x, BigInt* y, BigInt* gcd) {
    BigInt zero = {{0}, false};
    if (compareBigInt(b, &zero) == 0) {
        copyBigInt(a, gcd);
        initializeBigInt(x);
        x->value[0] = 1;  // x = 1
        initializeBigInt(y);  // y = 0
        return;
    }

    BigInt x1, y1, remainder, gcdTemp, quotient;
    initializeBigInt(&x1);
    initializeBigInt(&y1);
    initializeBigInt(&remainder);
    initializeBigInt(&gcdTemp);
    initializeBigInt(&quotient);

    divideBigInt(a, b, &quotient, &remainder);
    gcdExtendedBigInt(b, &remainder, &x1, &y1, &gcdTemp);

    // x = y1
    copyBigInt(&y1, x);
    // y = x1 - quotient * y1
    BigInt temp;
    initializeBigInt(&temp);
    multiplyBigInt(&quotient, &y1, &temp);
    subtractBigInt(&x1, &temp, y);

    copyBigInt(&gcdTemp, gcd);
}

void modInverseBigInt(const BigInt* a, const BigInt* mod, BigInt* inverse) {
    BigInt x, y, gcd;
    initializeBigInt(&x);
    initializeBigInt(&y);
    initializeBigInt(&gcd);

    gcdExtendedBigInt(a, mod, &x, &y, &gcd);

    BigInt one = {{1}, false};
    if (compareBigInt(&gcd, &one) != 0) {
        printf("Error: Modular inverse does not exist.\n");
        initializeBigInt(inverse);
        return;
    }

    // 修正负数逆元
    if (x.isNegative) {
        addBigInt(&x, mod, inverse);
    } else {
        copyBigInt(&x, inverse);
    }
    inverse->isNegative = false;
}

// 大整数取模运算
void modBigInt(const BigInt* num, const BigInt* mod, BigInt* result) {
    BigInt quotient, remainder;
    initializeBigInt(&quotient);
    initializeBigInt(&remainder);

    divideBigInt(num, mod, &quotient, &remainder);
    //printf("除数:%08x %08x %08x %08x\n", quotient.value[3], quotient.value[2], quotient.value[1], quotient.value[0]);
    copyBigInt(&remainder, result);
    
    if (result->isNegative) {
	    addBigInt(result, mod, result);
	    result->isNegative = false;
	}
	
}

// 大整数模幂运算: base^exponent % mod
void modExponentiation(const BigInt* base, const BigInt* exponent, const BigInt* mod, BigInt* result) {
    BigInt tempBase, tempExponent, tempResult, temp;
    initializeBigInt(&tempBase);
    initializeBigInt(&tempExponent);
    initializeBigInt(&tempResult);
    initializeBigInt(&temp);
	int i = 0; 
    // tempResult = 1 初始化
    tempResult.value[0] = 1;

    // 先做 base % mod，避免 base 太大
    modBigInt(base, mod, &tempBase);

    // 复制 exponent 到 tempExponent
    copyBigInt(exponent, &tempExponent);

    // 开始幂运算
    while (compareBigInt(&tempExponent, &((BigInt){{0}, false})) > 0) {
        // 如果 exponent 是奇数，result = (result * base) % mod
        if ((tempExponent.value[0] & 1) == 1) {
            multiplyBigInt(&tempResult, &tempBase, &temp);  // tempResult = tempResult * tempBase
            modBigInt(&temp, mod, &tempResult);  // tempResult = (tempResult * tempBase) % mod
            printf("result:");
            int j = 8;
		    while(j){
		    	j--;
		    	
				printf("%08x", tempResult.value[j]);
			}
			printf("\n");
		}

        // tempExponent >>= 1 右移
        bigIntRightShift(&tempExponent);
		i++;
        // tempBase = (tempBase * tempBase) % mod
        multiplyBigInt(&tempBase, &tempBase, &temp);  // tempBase = tempBase * tempBase
        modBigInt(&temp, mod, &tempBase);  // tempBase = (tempBase * tempBase) % mod
        printf("%d",i); 
        printf("base:");
		int j = 8;
	    while(j){
	    	j--;
	    	
			printf("%08x", tempBase.value[j]);
		}
        printf("\n");
    }

    // 最终的 result 存储到 output
    copyBigInt(&tempResult, result);
}
/*
void montgomeryMultiply(const BigInt* a, const BigInt* b, const BigInt* mod ,BigInt* modinv,BigInt* result,int RK) {
    BigInt tempc,temp; 
    initializeBigInt(&tempc);
    initializeBigInt(&temp);
    multiplyBigInt(a,b,&tempc);
    temp = montgomeryReduce(&tempc, mod, modinv, RK);
    copyBigInt(result, &temp);
}

BigInt montgomeryReduce(BigInt* x, const BigInt* mod, BigInt* mod_inv, int Rk) {
    BigInt m,t,t1;
    initializeBigInt(&m);
    initializeBigInt(&t);
    initializeBigInt(&t1);
    multiplyBigInt(x, mod_inv, &m);
    m = mod_2_k(m, Rk);

    multiplyBigInt(&m, mod,&t);
    addBigInt(x, &t,&t1);
    t = division_2_k(t1, Rk);
    return t;
}

// 计算a^e % n，通过蒙哥马利快速幂运算
void montgomeryExponentiation(const BigInt* base, const BigInt* exponent, const BigInt* mod, BigInt* result, BigInt* R) {
    BigInt tempBase = *base;
    BigInt tempExponent = *exponent;
    BigInt mod_inv,aR;
    initializeBigInt(&mod_inv);
    initializeBigInt(&aR);
    initializeBigInt(result);
    result->value[0] = 1;  // 初始值为 1

    modInverseBigInt(mod,R,&mod_inv);
    // 计算除数 R 的有效位数
    int RBits = 0;
    for (int i = MAX_SIZE - 1; i >= 0; i--) {
        if (R->value[i] != 0) {
            RBits = (i + 1) * 32 - clz(R->value[i]);
            break;
        }
    }
    multiplyBigInt(&tempBase,R,&aR);

    // 快速幂
    while (compareBigInt(&tempExponent, &((BigInt) { { 0 }, false })) > 0) {
        if (tempExponent.value[0] % 2 == 1) {
            montgomeryMultiply(result, &tempBase, mod, &mod_inv, result, RBits);  // result = result * base % mod
        }
        montgomeryMultiply(&tempBase, &tempBase, mod, &mod_inv, result, RBits);  // base = base * base % mod
        tempExponent.value[0] /= 2;
    }
}

// Miller-Rabin 素性检验
int montgomeryMillerRabinTest(const BigInt* n, int k) {
    // 如果 n 小于 2，直接返回不素数
    if (compareBigInt(n, &((BigInt) { { 2 }, false })) <= 0) {
        return 0; // n <= 2, not prime
    }

    // 计算 n - 1 = 2^s * d，得到 s 和 d
    BigInt nMinusOne, d;
    initializeBigInt(&nMinusOne);
    initializeBigInt(&d);
    subtractBigInt(n, &((BigInt) { { 1 }, false }), &nMinusOne);

    // 计算 2^s
    int s = 0;
    while ((nMinusOne.value[0] & 1) == 0) {
        s++;
        nMinusOne.value[0] >>= 1;
    }
    d = nMinusOne;

    // 选择 k 次随机数进行测试
    BigInt R, Rinv;
    initializeBigInt(&R);
    initializeBigInt(&Rinv);
    BigInt a, x, temp;
    for (int i = 0; i < k; i++) {
        // 生成一个随机数 a 使得 2 <= a <= n - 2
        // (这里假设一个生成随机数的函数：generateRandomBigInt)
        //generateRandomBigInt(&a, n);

        // 计算 a^d % n，得到 x
        montgomeryExponentiation(&a, &d, n, &x, &R);

        if (compareBigInt(&x, &((BigInt) { { 1 }, false })) == 0 || compareBigInt(&x, n) == 0) {
            continue;  // 如果 x == 1 或 x == n-1，则继续下一轮
        }

        // 在测试的 k 次中，如果 x 不是 1 或 n-1，则执行迭代判断
        for (int r = 0; r < s - 1; r++) {
            montgomeryMultiply(&x, &x, n, &temp, &R, &R);  // x = x^2 % n
            if (compareBigInt(&temp, &((BigInt) { { 1 }, false })) == 0) {
                return 0;  // 发现 1，说明 n 不是素数
            }
            if (compareBigInt(&temp, n) == 0) {
                break;  // 找到 n-1 说明继续判断
            }
        }
    }
    return 1;  // 如果没有找到合适的证据，则认为 n 是素数
}

// 费马素性测试
int montgomeryFermatTest(const BigInt* n, int k) {
    // 如果 n 小于 2，直接返回不素数
    if (compareBigInt(n, &((BigInt) { { 2 }, false })) <= 0) {
        return 0; // n <= 2, not prime
    }

    // 计算 n - 1
    BigInt nMinusOne;
    initializeBigInt(&nMinusOne);
    subtractBigInt(n, &((BigInt) { { 1 }, false }), &nMinusOne);

    // 蒙哥马利域的辅助值
    BigInt R, Rinv;
    initializeBigInt(&R);
    initializeBigInt(&Rinv);
    BigInt a, x;
    for (int i = 0; i < k; i++) {
        // 计算 a^(n-1) % n
        montgomeryExponentiation(&a, &nMinusOne, n, &x, &R);

        if (compareBigInt(&x, &((BigInt) { { 1 }, false })) != 0) {
            // 如果 a^(n-1) % n != 1，则 n 不是素数
            return 0;
        }
    }
    return 1;  // 如果没有找到合适的证据，则认为 n 是素数
}
*/
int main() {
    
    BigInt b, d;
	//initializeBigInt(&a);
	initializeBigInt(&b);
	//initializeBigInt(&c);
    initializeBigInt(&d);
    /*
    a.value[0] = 0xb444bf57;
    a.value[1] = 0x28c6206a;
    //b.value[0] = 0x55a77728;
    //b.value[1] = 0x2e248ced;
	//c.value[1] = 0x2e248cee;
    //c.value[1] = 0xffffffff;
    b.value[0] = 0x9a7d4021;
    b.value[1] = 0x1d8199ec;
	c.value[0] = 0x2f06789b;
	c.value[1] = 0x2e248cee;
	*/
	BigInt a = { {0x9916829e, 0xe82fdf82, 0x74b7dfae, 0x90347b07, 0xeb8a082a, 0x3d5c1d2d, 0x88180562, 0x8df9b77d}, false};
	BigInt c = { {0xb3b4b27b, 0x2a9237a5, 0xc5e1276, 0x7edf6b04, 0xe0c60377, 0x65abde9c, 0xb16e20b7, 0xb9a6ef75}, false};
	b.value[0] = 65537;
	//multiplyBigInt(&c,&c,&d);
	//modBigInt(&d,&a,&b);
	modExponentiation(&a,&b,&c,&d);
    //montgomeryExponentiation(&a,&b,&c,&d,&R_256);
    /*
     while (compareBigInt(&b, &((BigInt){{0}, false})) > 0){
     	int tmp = b.value[0] & 1;
		printf("%d",tmp);
	 	bigIntRightShift(&b);
	 }*/
     	
    //printf("逆元:%08x %08x %08x %08x\n", c.value[3], c.value[2], c.value[1], c.value[0]);
    int i = 8;
    while(i){
    	i--;
		printf("%08x", d.value[i]);
	}
    	
	return 0;
}
