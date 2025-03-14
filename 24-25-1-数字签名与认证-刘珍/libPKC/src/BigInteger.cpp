//
// Created by 张笑宇 on 2024/10/12.
//

#include "BigInteger.h"



BigInteger::BigInteger() {
    num = static_cast<char *>(malloc(2 * sizeof(char)));
    strcpy(num, "0");
}

BigInteger::BigInteger(const int &num) {
    const std::string strNum = std::to_string(num);
    this->num = static_cast<char *>(malloc((strNum.length() + 1) * sizeof(char)));
    strcpy(this->num, strNum.c_str());
    isNegative = num < 0;
}

BigInteger::BigInteger(const char *&str) {
}

BigInteger::~BigInteger() {
    free(num);
}

std::ostream & operator<<(std::ostream &os, const BigInteger &bi) {
    return bi.isNegative ? os << "-" << bi.num : os << bi.num;
}
