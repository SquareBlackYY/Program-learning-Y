//
// Created by 张笑宇 on 2024/9/29.
//

#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
#include <utility>
#include <vector>
#include <gmpxx.h>
#include <openssl/evp.h>
using namespace std;

/**
 * 快速模幂
 * @param base 底数
 * @param power 指数
 * @param mod 模数
 * @return
 */
mpz_class powm(const mpz_class &base, const mpz_class &power, const mpz_class &mod);

/**
 * 生成随机大整数[a, b)
 * @param lowerBound 下限
 * @param upperBound 上限
 * @return
 */
mpz_class getRandNum(const mpz_class &lowerBound, const mpz_class &upperBound);

/**
 * 生成随机大整数[a, b)，且与coprime互素
 * @param lowerBound 下限
 * @param upperBound 上限
 * @param coprime 生成结果与这个数互素
 * @return
 */
mpz_class getRandNumWithCoprime(const mpz_class &lowerBound, const mpz_class &upperBound, const mpz_class &coprime);

/**
 * 扩展欧几里得算法
 * @return {x, y, gcd}，其中 gcd = ax + by
 */
vector<mpz_class> exEuclid(const mpz_class &a, const mpz_class &b);

/**
 * 求最大公因数
 * @return a 和 b 的最大公因数
 */
mpz_class gcd(const mpz_class &a, const mpz_class &b);

/**
 * 扩展欧里几德计算 a ^ -1 mod b
 * @param a 可以传入负数，自动处理
 * @param b 只能传入正数
 * @return 返回 0 时 a, b 不互素
 */
mpz_class getInv(const mpz_class &a, const mpz_class &b);

/** 判断两个数是否互素
 */
bool isCoprime(const mpz_class &a, const mpz_class &b);

/**
 * 字符串转整数
 * @param str UTF-8字符串
 * @return 大整数表示的字节串
 */
mpz_class stringToMpz(const string &str);

/**
 * 整数转字符串
 * @param num 大整数表示的字节串
 * @return UTF-8字符串
 */
string mpzToString(const mpz_class &num);

/**
 * 哈希函数SHA256
 * @param input 大整数
 * @return
 */
mpz_class sha256(const mpz_class &input);

/**
 * 哈希函数SHA256
 * @param input 字符串
 * @return 64字符长度16进制字符串
 */
string sha256(const string &input);

#endif //TOOLS_H
