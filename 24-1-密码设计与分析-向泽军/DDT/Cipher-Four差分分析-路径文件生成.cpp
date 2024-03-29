#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>
using namespace std;

#define sbox_length 16

const int sbox[sbox_length] = {0x6, 0x4, 0xc, 0x5, 0x0, 0x7, 0x2, 0xe, 0x1, 0xf, 0x3, 0xd, 0x8, 0xa, 0x9, 0xb};
unsigned long long power_16[17];
int sbox_diff_table[sbox_length][sbox_length] = {0};
int difference;
int path_line[8];
double probability = 1e-10;

void generate_sbox_diff_table();
void round_f(int, int, int, unsigned long long, int, ofstream &);
int subByte(int);

int main()
{
    generate_sbox_diff_table();
    ostringstream oss;
    for (difference = 0x0001; difference <= 0x0001; difference++)
    {
        oss.str("");
        oss << "path-" << hex << setw(4) << setfill('0') << difference << ".csv";
        ofstream outfile(oss.str());
        outfile << "1box,1sub,2box,2sub,3box,3sub,4box,4sub,prob,prob_t" << "\n";
        round_f(difference, 4, 0, 1, 0, outfile);
        outfile.close();
    }
    return 0;
}

void generate_sbox_diff_table()
{
    int x, i;
    for (x = 0; x < sbox_length; x++)
        for (i = 0; i < sbox_length; i++)
            sbox_diff_table[x][sbox[i] ^ sbox[i ^ x]]++;
    power_16[0] = 1;
    power_16[16] = 0 - 1;
    for (i = 1; i < 16; i++)
        power_16[i] = power_16[i - 1] << 4;
}

void round_f(int m, int round_num, int sbox_num, unsigned long long pro_Numerator, int pro_Denominator_power, ofstream &outfile)
{
    int i, sbox_input, sbox_element, m_subed;
    double new_probability;
    if (round_num == 0)
    {
        for (i = 0; i < 8; i++)
            outfile << hex << setfill('0') << setw(4) << path_line[i] << ",";
        //outfile << dec << pro_Numerator << "," << pro_Denominator_power << "\n";
        outfile << dec << static_cast<double>(pro_Numerator)/power_16[pro_Denominator_power] << "\n";
        return;
    }
    else
    {
        if (sbox_num == 0)
        {
            sbox_input = m >> 12;
            if (sbox_input == 0)
            {
                round_f(m, round_num, sbox_num + 1, pro_Numerator, pro_Denominator_power, outfile);
            }
            else
            {
                for (i = 0; i < sbox_length; i++)
                {
                    sbox_element = sbox_diff_table[sbox_input][i];
                    if (sbox_element > 0)
                    {
                        m = m & 0x0fff ^ i << 12;
                        round_f(m, round_num, sbox_num + 1, pro_Numerator * sbox_element, pro_Denominator_power + 1, outfile);
                    }
                }
            }
        }
        else if (sbox_num == 1)
        {
            sbox_input = m >> 8 & 0xf;
            if (sbox_input == 0)
            {
                round_f(m, round_num, sbox_num + 1, pro_Numerator, pro_Denominator_power, outfile);
            }
            else
            {
                for (i = 0; i < sbox_length; i++)
                {
                    sbox_element = sbox_diff_table[sbox_input][i];
                    if (sbox_element > 0)
                    {
                        m = m & 0xf0ff ^ i << 8;
                        round_f(m, round_num, sbox_num + 1, pro_Numerator * sbox_element, pro_Denominator_power + 1, outfile);
                    }
                }
            }
        }
        else if (sbox_num == 2)
        {
            sbox_input = m >> 4 & 0xf;
            if (sbox_input == 0)
            {
                round_f(m, round_num, sbox_num + 1, pro_Numerator, pro_Denominator_power, outfile);
            }
            else
            {
                for (i = 0; i < sbox_length; i++)
                {
                    sbox_element = sbox_diff_table[sbox_input][i];
                    if (sbox_element > 0)
                    {
                        m = m & 0xff0f ^ i << 4;
                        round_f(m, round_num, sbox_num + 1, pro_Numerator * sbox_element, pro_Denominator_power + 1, outfile);
                    }
                }
            }
        }
        else
        {
            sbox_input = m & 0xf;
            if (sbox_input == 0)
            {
                path_line[8 - 2 * round_num] = m;
                m_subed = subByte(m);
                path_line[8 - 2 * round_num + 1] = m_subed;
                round_f(m_subed, round_num - 1, 0, pro_Numerator, pro_Denominator_power, outfile);
            }
            else
            {
                for (i = 0; i < sbox_length; i++)
                {
                    sbox_element = sbox_diff_table[sbox_input][i];
                    if (sbox_element > 0)
                    {
                        m = m & 0xfff0 ^ i;
                        path_line[8 - 2 * round_num] = m;
                        m_subed = subByte(m);
                        path_line[8 - 2 * round_num + 1] = m_subed;
                        round_f(m_subed, round_num - 1, 0, pro_Numerator * sbox_element, pro_Denominator_power + 1, outfile);
                    }
                }
            }
        }
    }
    return;
}

int subByte(int input)
{
    int a, b, c, d;
    a = input >> 12 & 0x8 ^ input >> 9 & 0x4 ^ input >> 6 & 0x2 ^ input >> 3 & 0x1;
    b = input >> 11 & 0x8 ^ input >> 8 & 0x4 ^ input >> 5 & 0x2 ^ input >> 2 & 0x1;
    c = input >> 10 & 0x8 ^ input >> 7 & 0x4 ^ input >> 4 & 0x2 ^ input >> 1 & 0x1;
    d = input >> 9 & 0x8 ^ input >> 6 & 0x4 ^ input >> 3 & 0x2 ^ input & 0x1;
    return a << 12 ^ b << 8 ^ c << 4 ^ d;
}