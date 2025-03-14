//
// Created by 张笑宇 on 2024/10/12.
//

#ifndef BIGINTEGER_H
#define BIGINTEGER_H

#include <cstdlib>
#include <cstring>
#include <ostream>

class BigInteger {
    char* num = nullptr;
    bool isNegative = false;

public:
    /**
     * @brief 空构造，初始化值为0
     */
    BigInteger();

    explicit BigInteger(const int &num);

    explicit BigInteger(const char* &str);

    ~BigInteger();

    friend std::ostream &operator<<(std::ostream &os, const BigInteger &bi);
};



#endif //BIGINTEGER_H
