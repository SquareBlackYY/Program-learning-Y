#include <iostream>
using namespace std;

int main() {
    string num1 = "-1233435666";
    string num2 = "-2345667778";
    
    // 判断符号
    bool isNegative1 = (num1[0] == '-');
    bool isNegative2 = (num2[0] == '-');

    // 如果有符号则去掉符号部分
    if (isNegative1) num1 = num1.substr(1);
    if (isNegative2) num2 = num2.substr(1);
    
    // 将较短的数用前导零补齐
    while (num1.size() < num2.size()) num1 = "0" + num1;
    while (num2.size() < num1.size()) num2 = "0" + num2;

    string result = "";
    int carry = 0;

    // 如果两个数符号相同，直接进行加法
    if (isNegative1 == isNegative2) {
        for (int i = num1.size() - 1; i >= 0; --i) {
            int sum = (num1[i] - '0') + (num2[i] - '0') + carry;
            carry = sum / 10;
            result += (sum % 10) + '0';
        }
        if (carry) {
            result += '1';
        }
        reverse(result.begin(), result.end());

        // 如果是负数，给结果加上负号
        if (isNegative1) {
            result = "-" + result;
        }
    }
    // 如果两个数符号不同，执行减法操作
    else {
        bool isNum1Larger = num1 >= num2;  // 判断绝对值大小，决定大数减小数

        // 确定要用大数减去小数
        string larger = isNum1Larger ? num1 : num2;
        string smaller = isNum1Larger ? num2 : num1;

        for (int i = larger.size() - 1; i >= 0; --i) {
            int diff = (larger[i] - '0') - (smaller[i] - '0') - carry;
            if (diff < 0) {
                diff += 10;
                carry = 1;
            } else {
                carry = 0;
            }
            result += diff + '0';
        }

        // 去除前导零
        while (result.size() > 1 && result.back() == '0') {
            result.pop_back();
        }

        reverse(result.begin(), result.end());

        // 如果num1的绝对值大且是负数，或者num2的绝对值大且是负数，则结果为负数
        if ((isNum1Larger && isNegative1) || (!isNum1Larger && isNegative2)) {
            result = "-" + result;
        }
    }

    cout << result << endl;

    return 0;
}