#include <iostream>
#include <string>
#include <bitset>
#include <algorithm>
#include <sstream>
#include <iomanip>

// Helper function to convert a hex character to its binary representation (4 bits)
std::string hexCharToBin(char hex) {
    switch (hex) {
        case '0': return "0000";
        case '1': return "0001";
        case '2': return "0010";
        case '3': return "0011";
        case '4': return "0100";
        case '5': return "0101";
        case '6': return "0110";
        case '7': return "0111";
        case '8': return "1000";
        case '9': return "1001";
        case 'A': return "1010";
        case 'B': return "1011";
        case 'C': return "1100";
        case 'D': return "1101";
        case 'E': return "1110";
        case 'F': return "1111";
        default: return "";
    }
}

// Helper function to convert a binary string (4 bits) to a hex character
char binToHexChar(const std::string &bin) {
    if (bin == "0000") return '0';
    if (bin == "0001") return '1';
    if (bin == "0010") return '2';
    if (bin == "0011") return '3';
    if (bin == "0100") return '4';
    if (bin == "0101") return '5';
    if (bin == "0110") return '6';
    if (bin == "0111") return '7';
    if (bin == "1000") return '8';
    if (bin == "1001") return '9';
    if (bin == "1010") return 'A';
    if (bin == "1011") return 'B';
    if (bin == "1100") return 'C';
    if (bin == "1101") return 'D';
    if (bin == "1110") return 'E';
    if (bin == "1111") return 'F';
    return ' ';
}

int main() {
    std::string hexInput;
    std::cout << "Enter 16 hexadecimal characters: ";
    std::cin >> hexInput;

    if (hexInput.length() != 16) {
        std::cerr << "Invalid input length. Please enter exactly 16 hexadecimal characters." << std::endl;
        return 1;
    }

    std::string binaryString;

    // Convert hex to binary
    for (char hexChar : hexInput) {
        binaryString += hexCharToBin(hexChar);
    }

    // Reverse the binary string
    std::reverse(binaryString.begin(), binaryString.end());

    // Convert binary back to hex
    std::string hexOutput;
    for (size_t i = 0; i < binaryString.size(); i += 4) {
        std::string binSegment = binaryString.substr(i, 4);
        hexOutput += binToHexChar(binSegment);
    }

    std::cout << "Reversed bit order hex: " << hexOutput << std::endl;

    return 0;
}
