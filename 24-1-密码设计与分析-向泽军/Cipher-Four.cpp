#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

#define sbox_length 16

const int sbox[sbox_length] = {0x6, 0x4, 0xc, 0x5, 0x0, 0x7, 0x2, 0xe, 0x1, 0xf, 0x3, 0xd, 0x8, 0xa, 0x9, 0xb};
int sbox_diff_table[sbox_length][sbox_length] = {0};
int sbox_status[8] = {0};
int difference;

void generate_sbox_diff_table();
void round_f(int, int, int, int, int, ofstream &);
void subByte(int &);

int main()
{
    generate_sbox_diff_table();
    int i;
    ostringstream oss;
    for (difference = 0xffff; difference <= 0xffff; difference++)
    {
        oss.str("");
        oss << "path-" << hex << setw(4) << setfill('0') << difference << ".csv";
        ofstream outfile(oss.str());
        outfile << "1box,1sub,2box,2sub,3box,3sub,4box,4sub,prob,prob_t" << "\n";
        round_f(difference, 1, 0, 1, 0, outfile);
        outfile.close();
    }
    return 0;
}

void generate_sbox_diff_table()
{
    int x, i;
    for(x = 0; x < sbox_length; x++)
        for(i = 0; i < sbox_length; i++)
            sbox_diff_table[x][sbox[i] ^ sbox[i ^ x]]++;
}

void round_f(int m, int round_num, int sbox_num, int probability, int probability_time, ofstream &outfile)
{
    int i, sbox_element;
    if (round_num == 0)
    {
        outfile << probability << "," << probability_time << "\n";
        return;
    }
    else
        if (sbox_num == 0)
            for (i = 0; i < sbox_length; i++)
            {
                sbox_element = sbox_diff_table[m >> 12][i];
                if (sbox_element > 0)
                {
                    probability_time++;
                    probability *= sbox_element;
                    m = m & 0x0fff ^ i << 12;
                    cout << sbox_element << " " << m << " " << round_num << " " << sbox_num << " " << probability << " " << probability_time << endl;
                    round_f(m, round_num, sbox_num + 1, probability, probability_time, outfile);
                }
            }
        else if (sbox_num == 1)
            for (i = 0; i < sbox_length; i++)
            {
                sbox_element = sbox_diff_table[m >> 8 & 0xf][i];
                if (sbox_element > 0)
                {
                    probability_time++;
                    probability *= sbox_element;
                    m = m & 0xf0ff ^ i << 8;
                    round_f(m, round_num, sbox_num + 1, probability, probability_time, outfile);
                }
            }
        else if (sbox_num == 2)
            for (i = 0; i < sbox_length; i++)
            {
                sbox_element = sbox_diff_table[m >> 4 & 0xf][i];
                if (sbox_element > 0)
                {
                    probability_time++;
                    probability *= sbox_element;
                    m = m & 0xff0f ^ i << 4;
                    round_f(m, round_num, sbox_num + 1, probability, probability_time, outfile);
                }
            }
        else
            for (i = 0; i < sbox_length; i++)
            {
                sbox_element = sbox_diff_table[m & 0xf][i];
                if (sbox_element > 0)
                {
                    probability_time++;
                    probability *= sbox_element;
                    m = m & 0xfff0 ^ i;
                    sbox_status[8 - round_num * 2] = m;
                    subByte(m);
                    sbox_status[8 - round_num * 2 + 1] = m;
                    for (int i = 0; i < 8; i++)
                        outfile << setfill('0') << setw(4) << hex << sbox_status[i] << ",";;
                    round_f(m, round_num - 1, 0, probability, probability_time, outfile);
                }
            }
    return;
}

void subByte(int &input)
{
    int a, b, c, d;
    a = input >> 12 & 0x8 ^ input >> 9 & 0x4 ^ input >> 6 & 0x2 ^ input >> 3 & 0x1;
    b = input >> 11 & 0x8 ^ input >> 8 & 0x4 ^ input >> 5 & 0x2 ^ input >> 2 & 0x1;
    c = input >> 10 & 0x8 ^ input >> 7 & 0x4 ^ input >> 4 & 0x2 ^ input >> 1 & 0x1;
    d = input >> 9 & 0x8 ^ input >> 6 & 0x4 ^ input >> 3 & 0x2 ^ input & 0x1;
    input = a << 12 ^ b << 8 ^ c << 4 ^ d;
}