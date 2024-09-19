#include <iostream>
#include <sstream>
using namespace std;

class BigInteger
{
private:
    string number;   // 数字部分
    size_t point;    // 小数点位置 10 ^ -point
    bool isNegative; // 是否为负数

    const static size_t k = 5; // 除法最多延长k位

    // 化简数字部分（去除前后多余的0）
    static void simplifyInt(BigInteger &bi)
    {
        size_t front, back;          // 前后首个非零字符位置
        bool isFirstNotZero = false; // 是否遇到首个非零字符

        // 判断首个非0字符位置
        for (size_t i = 0; i < bi.number.length(); ++i)
            if (bi.number[i] != '0')
            {
                isFirstNotZero = true;
                front = i;
                break;
            }

        // 判断是否全0
        if (!isFirstNotZero)
        {
            bi.number = "0";
            bi.point = 0;
            bi.isNegative = false;
        }
        else
        {
            // 去除小数部分多余的0
            size_t i = 0;
            for (; i < bi.number.length(); ++i)
            {
                if (bi.number[bi.number.length() - 1 - i] == '0' && bi.point > 0)
                    --bi.point;
                else
                    break;
            }
            back = bi.number.length() - i - 1;

            // 写入字符串
            bi.number = bi.number.substr(front, back - front + 1);
        }
    }

    // 对齐两个数字（后补0）
    static void alignInt(BigInteger &a, BigInteger &b)
    {
        BigInteger *minPointPart = a.point < b.point ? &a : &b;
        const size_t diffPoint = a.point > b.point ? a.point - b.point : b.point - a.point;
        minPointPart->number.append(diffPoint, '0');
        minPointPart->point += diffPoint;
    }

    // 辅助函数：字符串加法
    static string addStrings(BigInteger &a, BigInteger &b)
    {
        string result;
        int carry = 0, sum = 0; // 进位与和

        alignInt(a, b);
        int i = a.number.length() - 1;
        int j = b.number.length() - 1;

        while (i >= 0 || j >= 0 || carry)
        {
            int x = (i >= 0) ? a.number[i--] - '0' : 0;
            int y = (j >= 0) ? b.number[j--] - '0' : 0;
            sum = x + y + carry;
            carry = sum / 10;
            result.push_back((sum % 10) + '0');
        }
        reverse(result.begin(), result.end());
        return result;
    }

    // 辅助函数：字符串减法（绝对值大减小）
    static string subtractStrings(BigInteger &a, BigInteger &b)
    {
        string result;
        int borrow = 0, diff = 0; // 借位与差

        alignInt(a, b);
        int i = a.number.length() - 1, j = b.number.length() - 1;

        while (i >= 0 || j >= 0)
        {
            int x = (i >= 0) ? a.number[i--] - '0' : 0;
            int y = (j >= 0) ? b.number[j--] - '0' : 0;
            diff = x - y - borrow;
            if (diff < 0)
            {
                diff += 10;
                borrow = 1;
            }
            else
                borrow = 0;
            result.push_back(diff + '0');
        }
        reverse(result.begin(), result.end());
        return result;
    }

    // 比较两个函数的绝对值，a > b时返回1，<时返回0，相等时返回-1
    static int compareAbsValue(BigInteger &a, BigInteger &b)
    {
        simplifyInt(a);
        simplifyInt(b);
        alignInt(a, b);
        
        if (a.number.length() > b.number.length())
            return true;
        else if (a.number.length() < b.number.length())
            return false;
        else
        {
            for (size_t i = 0; i < a.number.length(); ++i)
            {
                if (a.number[i] > b.number[i])
                    return true;
                else if (a.number[i] < b.number[i])
                    return false;
            }
            return -1;
        }
    }

public:
    // 空参构造
    BigInteger() : number("0"), point(0), isNegative(false) {};

    // 传入整数或小数构造
    BigInteger(double input)
    {
        // 将 double 转换为字符串
        ostringstream oss;
        oss << input;
        string str = oss.str();

        // 调用字符串构造函数
        *this = BigInteger(str);
    }

    // 传入字符串构造
    explicit BigInteger(const string &str)
    {
        const size_t length = str.length(); // 输入字符串长度

        // 不允许输入空字符串进行构造
        if (!length)
            throw invalid_argument("Empty string is valid");

        // 检查符号
        this->isNegative = false;
        bool isSymbol = true; // 输入字符串是否有符号
        if (str[0] == '+')
            ;
        else if (str[0] == '-')
            this->isNegative = true;
        else if (isdigit(str[0]))
            isSymbol = false;
        else
            throw invalid_argument("Valid String");

        // 数字部分检查
        this->point = 0;      // 小数点位置
        bool isPoint = false; // 是否有小数点
        for (size_t i = (size_t)isSymbol; i < length; i++)
        {
            if (str[i] == '.' && this->point == 0)
            {
                this->point = length - i - 1;
                isPoint = true;
            }
            else if (isdigit(str[i]))
                ;
            else
                throw invalid_argument("Valid String");
        }

        // 写入数字部分
        string integerPart = str.substr((size_t)isSymbol, length - (size_t)isSymbol - point - (size_t)isPoint);
        if (point > 0)
        {
            string decimalPart = str.substr(length - point, point);
            this->number = integerPart + decimalPart;
        }
        else
            this->number = integerPart;

        // 化简数字部分
        simplifyInt(*this);
    }

    // 重载 = 运算符
    template <typename T>
    BigInteger &operator=(const T &other)
    {
        BigInteger result(other);
        *this = result;
        return *this;
    }
    BigInteger &operator=(const BigInteger &other)
    {
        // 检测自我赋值
        if (this == &other)
            return *this;

        // 拷贝每个成员变量
        this->number = other.number;
        this->point = other.point;
        this->isNegative = other.isNegative;

        // 返回当前对象以支持链式赋值
        return *this;
    }

    // 重载 ! 运算符，进行变号
    BigInteger operator!() const
    {
        BigInteger result = *this;

        // 判断是否是0，如果是0不做任何修改
        if (result.number != "0")
            result.isNegative = !this->isNegative; // 取反符号

        return result;
    }

    // 重载 == 运算符
    template <typename T>
    bool operator==(const T &other) const
    {
        BigInteger a = *this, b(other);
        simplifyInt(a);
        return (a.number == b.number &&
                a.point == b.point &&
                a.isNegative == b.isNegative);
    }
    bool operator==(const BigInteger &other) const
    {
        BigInteger a = *this, b = other;
        simplifyInt(a);
        simplifyInt(b);
        return (a.number == b.number &&
                a.point == b.point &&
                a.isNegative == b.isNegative);
    }

    // 重载 != 运算符
    template <typename T>
    bool operator!=(const T &other) const
    {
        BigInteger a = *this, b(other);
        simplifyInt(a);
        return (a.number != b.number ||
                a.point != b.point ||
                a.isNegative != b.isNegative);
    }
    bool operator!=(const BigInteger &other) const
    {
        BigInteger a = *this, b = other;
        simplifyInt(a);
        simplifyInt(b);
        return (this->number != other.number ||
                this->point != other.point ||
                this->isNegative != other.isNegative);
    }

    // 重载 >> 运算符
    friend istream &operator>>(istream &is, BigInteger &bi)
    {
        string str;
        is >> str; // 从输入流中获取字符串

        // 使用从输入流中获取的字符串调用构造函数
        bi = BigInteger(str);

        return is;
    }

    // 重载 << 运算符
    friend ostream &operator<<(ostream &os, const BigInteger &bi)
    {
        // 如果是负数，则输出负号
        if (bi.isNegative)
            os << '-';

        // 判断小数点位置
        if (bi.point < bi.number.length())
        {
            // 整数部分
            os << bi.number.substr(0, bi.number.length() - bi.point);

            // 小数部分
            if (bi.point)
                os << '.' + bi.number.substr(bi.number.length() - bi.point, bi.point);
        }
        else
        {
            os << "0.";
            for (size_t i = bi.number.length(); i < bi.point; ++i)
                os << '0';
            os << bi.number;
        }

        return os;
    }

    // 重载 + 运算符
    BigInteger operator+(const BigInteger &other) const
    {
        // 0判断
        if (*this == 0)
            return other;
        else if (other == 0)
            return *this;

        BigInteger result, a = *this, b = other;

        // 符号相同，直接相加
        if (a.isNegative == b.isNegative)
        {
            result.number = addStrings(a, b);
            result.isNegative = a.isNegative;
        }
        // 符号不同，转为减法
        else
        {
            int mode = compareAbsValue(a, b);
            if (mode)
            {
                result.number = subtractStrings(a, b);
                result.isNegative = a.isNegative;
            }
            else if (!mode)
            {
                result.number = subtractStrings(b, a);
                result.isNegative = b.isNegative;
            }
        }
        result.point = this->point;
        simplifyInt(result);
        return result;
    }

    // 重载 - 运算符
    BigInteger operator-(const BigInteger &other) const
    {
        // 0判断
        if (*this == 0)
            return !other;
        else if (other == 0)
            return *this;
        else if (*this == other)
            return BigInteger();

        BigInteger result, a = *this, b = other;

        // 符号不同，转为加法
        if (a.isNegative != b.isNegative)
        {
            result.number = addStrings(a, b);
            result.isNegative = a.isNegative;
        }
        // 符号相同，直接相减
        else
        {
            int mode = compareAbsValue(a, b);
            if (mode)
            {
                result.number = subtractStrings(a, b);
                result.isNegative = a.isNegative;
            }
            else if (!mode)
            {
                result.number = subtractStrings(b, a);
                result.isNegative = !a.isNegative;
            }
        }
        result.point = this->point;
        simplifyInt(result);
        return result;
    }

    // 重载 * 运算符
    BigInteger operator*(const BigInteger &other) const
    {

        BigInteger result;
        if (*this == 0 || other == 0)
            return result;

        BigInteger a = *this, b = other;

        const size_t n1 = a.number.length();
        const size_t n2 = b.number.length();
        const size_t resultLength = n1 + n2;
        int *product = new int[resultLength]();

        for (int i = n1 - 1; i >= 0; --i)
            for (int j = n2 - 1; j >= 0; --j)
            {
                int mul = (a.number[i] - '0') * (b.number[j] - '0');
                int sum = product[i + j + 1] + mul;
                product[i + j + 1] = sum % 10;
                product[i + j] += sum / 10;
            }

        for (size_t i = 0; i < resultLength; ++i)
            result.number.push_back(product[i] + '0');

        delete[] product;

        result.point = this->point + other.point;
        result.isNegative = this->isNegative != other.isNegative;
        simplifyInt(result);
        return result;
    }

    // 重载 / 运算符（最多延长k位）
    BigInteger operator/(const BigInteger &other) const
    {
        BigInteger result;

        if (*this == 0)
            return result;
        else if (other == 0)
            throw invalid_argument("Divide by zero error");

        BigInteger dividend = *this, divisor = other, remainder; // 除数、被除数、余数
        string quotient;                                         // 商

        // 将除数与被除数对齐为正整数
        alignInt(dividend, divisor);
        dividend.point = divisor.point = 0;
        dividend.isNegative = divisor.isNegative = false;

        const size_t maxLength = dividend.number.length() > divisor.number.length() ? dividend.number.length() : divisor.number.length();
        size_t i = 0;
        do
        {
            remainder.number.push_back(i < dividend.number.length() ? dividend.number[i] : '0');
            size_t count = 0;
            
            while (abs(compareAbsValue(remainder, divisor)) == 1)
            {
                remainder = remainder - divisor;
                count++;
            }

            quotient.push_back(count + '0');
        } while (i++ < maxLength + k);

        result.number = quotient;
        result.point = i > dividend.number.length() ? i - dividend.number.length() : 0;
        result.isNegative = this->isNegative != other.isNegative;
        simplifyInt(result);
        return result;
    }
};

int main()
{
    BigInteger a("20");
    BigInteger b("100");

    cout << "a = " << a << endl;
    cout << "b = " << b << endl;

    cout << "a + b = " << a + b << endl;
    cout << "a - b = " << a - b << endl;
    cout << "a * b = " << a * b << endl;
    cout << "a / b = " << a / b << endl;
    
    return 0;
}