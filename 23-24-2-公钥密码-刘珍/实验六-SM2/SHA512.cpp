#include <openssl/sha.h>
#include <iostream>
#include <iomanip>
#include <sstream>

std::string sha512(const std::string &str)
{
    SHA512_CTX ctx;
    SHA512_Init(&ctx);
    SHA512_Update(&ctx, str.c_str(), str.length());
    unsigned char hash[SHA512_DIGEST_LENGTH];
    SHA512_Final(hash, &ctx);
 
    std::stringstream ss;
    for (int i = 0; i < SHA512_DIGEST_LENGTH; ++i)
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    return ss.str();
}

int main()
{
    std::string input = "Hello, world!";
    std::string hash = sha512(input);
    std::cout << "SHA512 of '" << input << "' is: " << hash << std::endl;
    return 0;
}