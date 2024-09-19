#include <iostream>
#include <fstream>
#include <iomanip>

void printFirstPlaintextHex(const char *filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error opening file." << std::endl;
        return;
    }

    unsigned short int plaintext[4];
    file.read(reinterpret_cast<char *>(plaintext), sizeof(plaintext));
    file.close();

    std::cout << "First text in hexadecimal:" << std::endl;
    for (int i = 0; i < 4; ++i)
    {
        std::cout << std::hex << std::setw(4) << std::setfill('0') << plaintext[i] << std::endl;
    }
}

int main()
{
    const char *filename = "ciphertext.bin";
    printFirstPlaintextHex(filename);
    return 0;
}