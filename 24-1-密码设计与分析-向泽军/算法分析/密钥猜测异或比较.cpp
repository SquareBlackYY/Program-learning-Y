// differential_analysis.cpp

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

unsigned short int S[16] = {0x0, 0x4, 0x8, 0xf, 0x1, 0x5, 0xe, 0x9, 0x2, 0x7, 0xa, 0xc, 0xb, 0xd, 0x6, 0x3};
unsigned short int xorMask[4] = {0x8000, 0x0000, 0x0000, 0x0000};

void Sbox(unsigned short int state[4])
{
    unsigned short int temp1, temp2;
    temp1 = state[0];
    temp2 = state[1];
    state[0] = state[2] ^ (state[0] & state[1]);
    state[1] = state[3] ^ (state[1] & state[2]);
    state[2] = temp1 ^ (state[0] & state[1]);
    state[3] = temp2 ^ (state[1] & state[2]);
}

int main()
{
    ifstream file_in("ciphertext.bin", ios::in | ios::binary);

    if (!file_in.is_open()) {
        cerr << "Error opening input file!" << endl;
        return 1;
    }

    unsigned short int line1[4];
    unsigned short int line2[4];

    vector<unsigned long int> key_count(16, 0);

    while (file_in.read(reinterpret_cast<char*>(line1), sizeof(line1)) &&
           file_in.read(reinterpret_cast<char*>(line2), sizeof(line2))) {
        
        for (unsigned short int k = 0x0000, m = 0; m < 16; k += 0x1000, m++) {
            unsigned short int Key[4] = {k, 0x0000, 0x0000, 0x0000};

            unsigned short int temp1[4];
            unsigned short int temp2[4];

            // XOR with Key
            for (int i = 0; i < 4; ++i)
            {
                temp1[i] = line1[i] ^ Key[i];
                temp2[i] = line2[i] ^ Key[i];
            }

            // Apply Sbox
            Sbox(temp1);
            Sbox(temp2);

            // XOR the results of Sbox transformation
            bool is_match = true;
            for (int i = 0; i < 4; ++i)
            {
                unsigned short int xor_result = temp1[i] ^ temp2[i];
                if (i == 0)
                {
                    unsigned short int first_char = (xor_result >> 12) & 0xF;
                    if (first_char != (xorMask[i] >> 12))
                    {
                        is_match = false;
                        break;
                    }
                }
            }

            if (is_match) {
                key_count[k >> 12]++;
            }
        }
    }

    file_in.close();

    vector<pair<unsigned long int, int>> key_counts_with_indices;
    for (int i = 0; i < 16; ++i) {
        key_counts_with_indices.emplace_back(key_count[i], i);
    }

    sort(key_counts_with_indices.rbegin(), key_counts_with_indices.rend());

    cout << "keys:" << endl;
    for (const auto& kc : key_counts_with_indices) {
        cout << hex << kc.second << " ";
    }
    cout << endl;

    return 0;
}
