#include <iostream>
#include <cstdio>
#include <cstring>
#include <fstream>
using namespace std;

const uint8_t S_BOX[16] = {0xf, 0xe, 0xb, 0xc, 0x6, 0xd, 0x7, 0x8, 0x0, 0x3, 0x9, 0xa, 0x4, 0x2, 0x1, 0x5};

int main()
{
    int i, j, a, b, a_x, b_sx, x, table[16][16];
    memset(table, 0, 256 * sizeof(int));
    for (a = 0x0; a <= 0xf; a++)
        for (b = 0x0; b <= 0xf; b++)
            for (x = 0x0; x <= 0xf; x++)
            {
                a_x = 0x0, b_sx = 0x0;
                for (i = 0; i < 4; i++)
                {
                    a_x ^= ((a >> i) & 1) & ((x >> i) & 1);
                    b_sx ^= ((b >> i) & 1) & ((S_BOX[x] >> i) & 1);
                }
                if (a_x == b_sx)
                    table[a][b]++;
            }
    for (a = 0x0; a <= 0xf; a++)
        for (b = 0x0; b <= 0xf; b++)
            if (table[a][b] != 0)
                table[a][b] -= 8;
    ofstream outfile("LCT_AESsbox_4bit.csv");
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            outfile << table[i][j];
            if (j < 15)
                outfile << ",";
        }
        outfile << "\n";
    }
    outfile.close();
    return 0;
}