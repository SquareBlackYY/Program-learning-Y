#include <iostream>
#include <bitset>
#include <sstream>
#include <iomanip>

// 将十六进制字符串转换为二进制字符串
std::string hexToBin(const std::string& hex) {
    std::string binary;
    for (char ch : hex) {
        int n = std::stoi(std::string(1, ch), nullptr, 16);
        binary += std::bitset<4>(n).to_string();
    }
    return binary;
}

// 将二进制字符串转换回十六进制字符串
std::string binToHex(const std::string& bin) {
    std::stringstream ss;
    for (size_t i = 0; i < bin.length(); i += 4) {
        std::bitset<4> bits(bin.substr(i, 4));
        ss << std::hex << bits.to_ulong();
    }
    return ss.str();
}

int main() {
    std::string inputHex;
    std::cout << "输入一个8字符的十六进制字符串: ";
    std::cin >> inputHex;

    // 将输入的十六进制字符串转换为二进制
    std::string binStr = hexToBin(inputHex);

    // 逆序二进制字符串
    std::reverse(binStr.begin(), binStr.end());

    // 将逆序后的二进制转换回十六进制字符串
    std::string outputHex = binToHex(binStr);

    std::cout << "逆序后的十六进制字符串: " << outputHex << std::endl;

    return 0;
}