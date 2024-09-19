#include <iostream>
using namespace std;

class BigInteger
{
private:
    string number;      // 数字部分
    unsigned int point; // 小数点位置 10 ^ -point
    bool isNegative;    // 是否为负数

    // 化简数字部分（去除前后多余的0）
    static void simplifyInt(BigInteger &bi)
    {
        size_t front, back;          // 前后首个非零字符位置
        bool isFirstNotZero = false; // 是否遇到首个非零字符
        size_t addZeros = 0;         // 补0数量

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
            // 判断是否需要补0
            if (front >= bi.number.length() - bi.point)
                addZeros = front - bi.number.length() + bi.point + 1;

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
            bi.number.insert(0, addZeros, '0');
        }
    }

    // 对齐两个数字（前后补0）
    static void alignInt(BigInteger &a, BigInteger &b)
    {
        // 较小的数
        BigInteger *minIntPart = a.number.length() - a.point < b.number.length() - b.point ? &a : &b;
        BigInteger *minPointPart = a.point < b.point ? &a : &b;

        // 差值计算
        size_t diffInt = a.number.length() - a.point > b.number.length() - b.point ? a.number.length() - a.point - b.number.length() + b.point : b.number.length() - b.point - a.number.length() + a.point;
        size_t diffPoint = a.point > b.point ? a.point - b.point : b.point - a.point;

        // 整数部分填充
        minIntPart->number.insert(0, diffInt, '0');
        // 小数部分填充
        minPointPart->number.append(diffPoint, '0');
        minPointPart->point += diffPoint;
    }

    // 辅助函数：字符串加法
    static string addStrings(BigInteger &a, BigInteger &b)
    {
        string result;
        int carry = 0, sum = 0; // 进位与和

        alignInt(a, b);

        for (int i = a.number.length() - 1; i >= 0 || carry; --i)
        {
            int x = (i >= 0) ? a.number[i] - '0' : 0;
            int y = (i >= 0) ? b.number[i] - '0' : 0;
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

        for (int i = a.number.length() - 1; i >= 0; --i)
        {
            int x = (i >= 0) ? a.number[i] - '0' : 0;
            int y = (i >= 0) ? b.number[i] - '0' : 0;
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
        alignInt(a, b);
        for (size_t i = 0; i < a.number.length(); ++i)
        {
            if (a.number[i] > b.number[i])
                return true;
            else if (a.number[i] < b.number[i])
                return false;
        }
        return -1;
    }

public:
    // 空参构造
    BigInteger() : number("0"), point(0), isNegative(false) {};

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
    BigInteger &operator=(const string &other)
    {
        BigInteger result(other);
        return result;
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

        // 整数部分
        string integerPart = bi.number.substr(0, bi.number.length() - bi.point);
        os << integerPart;

        // 小数部分
        if (bi.point)
        {
            string decimalPart = bi.number.substr(bi.number.length() - bi.point, bi.point);
            os << '.' + decimalPart;
        }

        return os;
    }

    // 重载 + 运算符
    BigInteger operator+(const BigInteger &other) const
    {
        // 0判断
        if (this->number == "0")
            return other;
        else if (other.number == "0")
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
        result.point = a.point;
        simplifyInt(result);
        return result;
    }

    // 重载减法运算符
    BigInteger operator-(const BigInteger &other) const
    {
        // 0判断
        if (this->number == "0")
            return !other;
        else if (other.number == "0")
            return *this;

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
        result.point = a.point;
        simplifyInt(result);
        return result;
    }
};

int main()
{
    BigInteger a("-0000.00123000");
    BigInteger b("+0.1");
    cout << a << endl;
    cout << b << endl;

    cout << a - b << endl;
    return 0;
}