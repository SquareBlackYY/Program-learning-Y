#include <iostream>
#include <gmpxx.h>
#include <cstdint>

struct byteString int_to_bytes(const mpz_class &);
mpz_class bytes_to_int(const byteString &);

class byteString
{
public:
    size_t size;
    uint8_t *string;

    ~byteString()
    {
        delete[] string;
    }
    void PrintString()
    {
        std::cout << "字节串: ";
        for (size_t i = 0; i < size; i++)
            std::cout << std::hex << static_cast<int>(string[i]);
        std::cout << std::dec << std::endl;
    }
};

int main()
{
    mpz_class num("258");

    // 大整数转字节串
    byteString bytestring = int_to_bytes(num);

    bytestring.PrintString();

    // 字节串转大整数
    mpz_class result = bytes_to_int(bytestring);

    std::cout << std::dec << "大整数: " << result << std::endl;

    return 0;
}

struct byteString int_to_bytes(const mpz_class &Num)
{
    byteString bytestring;
    bytestring.size = (mpz_sizeinbase(Num.get_mpz_t(), 2) + 7) / 8;
    bytestring.string = new uint8_t[bytestring.size];
    mpz_export(bytestring.string, nullptr, 1, 1, 1, 0, Num.get_mpz_t());
    return bytestring;
}

mpz_class bytes_to_int(const byteString &bytestring)
{
    mpz_class Num;
    mpz_import(Num.get_mpz_t(), bytestring.size, 1, 1, 1, 0, bytestring.string);
    return Num;
}