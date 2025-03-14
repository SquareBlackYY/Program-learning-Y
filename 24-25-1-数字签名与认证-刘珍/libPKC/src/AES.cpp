//
// Created by 张笑宇 on 2024/10/13.
//

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <stdexcept>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <iostream>

#include "AES.h"

#include <iomanip>

#include "Tools.h"

using namespace std;

void generate_key_schedule(char *seed_key, uint8_t ***key_schedule) {
    char hex_text[3];
    uint8_t temp[16];
    *key_schedule = (uint8_t **) malloc(11 * sizeof(uint8_t *));
    for (int i = 0; i < 11; i++)
        (*key_schedule)[i] = (uint8_t *) malloc(16 * sizeof(uint8_t));

    for (int i = 0; i < 16; i++) {
        sscanf(seed_key + (i * 2), "%2s", hex_text);
        (*key_schedule)[0][i] = (uint8_t) strtol(hex_text, NULL, 16);
    }
    key_extend(*key_schedule);

    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 16; j++)
            temp[j] = (*key_schedule)[i][TRANSPOSE_TABLE[j]];
        memcpy((*key_schedule)[i], temp, 16 * sizeof(uint8_t));
    }
    printf("密钥扩展完成。\n");
}

void key_extend(uint8_t **key_schedule) {
    for (int round_num = 1; round_num < 11; round_num++) {
        memcpy(key_schedule[round_num], key_schedule[round_num - 1] + 12, 4 * sizeof(uint8_t));
        RotWord(key_schedule[round_num]);
        SubWord(key_schedule[round_num]);
        key_schedule[round_num][0] ^= key_schedule[round_num - 1][0] ^ R_CON[round_num - 1];
        for (int i = 1; i < 4; i++)
            key_schedule[round_num][i] ^= key_schedule[round_num - 1][i];
        for (int i = 4; i < 16; i++)
            key_schedule[round_num][i] = key_schedule[round_num][i - 4] ^ key_schedule[round_num - 1][i];
    }
}

void RotWord(uint8_t *key_word) {
    uint8_t temp = key_word[0];
    memcpy(key_word, key_word + 1, 3 * sizeof(uint8_t));
    key_word[3] = temp;
}

void SubWord(uint8_t *key_word) {
    for (int i = 0; i < 4; i++)
        key_word[i] = S_BOX[key_word[i]];
}

void AES_encrypt(int num_groups, uint8_t **input_text, uint8_t **key_schedule) {
    uint8_t *text;
    for (int i = 0; i < num_groups; i++) {
        text = input_text[i];
        AddRoundKey(text, key_schedule[0]);
        for (int round_num = 1; round_num < 10; round_num++) {
            subBytes(text);
            shiftRows(text);
            mixColumns(text);
            AddRoundKey(text, key_schedule[round_num]);
        }
        subBytes(text);
        shiftRows(text);
        AddRoundKey(text, key_schedule[10]);
    }
}

void AddRoundKey(uint8_t *text, uint8_t *key) {
    for (int i = 0; i < 16; i++)
        text[i] ^= key[i];
}

void subBytes(uint8_t *text) {
    for (int i = 0; i < 16; i++)
        text[i] = S_BOX[text[i]];
}

void shiftRows(uint8_t *text) {
    uint8_t temp[16];
    for (int i = 0; i < 16; i++)
        temp[i] = text[SHIFTROWS_TABLE[i]];
    memcpy(text, temp, 16 * sizeof(uint8_t));
}

void mixColumns(uint8_t *text) {
    uint8_t result[16];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            uint8_t mixed_num = 0;
            for (int k = 0; k < 4; k++) {
                if (MIXCOLUMNS_MATRIX[i][k] == 1)
                    mixed_num ^= text[k * 4 + j];
                else if (MIXCOLUMNS_MATRIX[i][k] == 2)
                    mixed_num ^= ((text[k * 4 + j] << 1) ^ ((text[k * 4 + j] >> 7) * 0x1B)) ^ 0xff;
                else if (MIXCOLUMNS_MATRIX[i][k] == 3)
                    mixed_num ^= ((text[k * 4 + j] << 1) ^ ((text[k * 4 + j] >> 7) * 0x1B) ^ text[k * 4 + j]) ^ 0xff;
            }
            result[i * 4 + j] = mixed_num;
        }
    }
    memcpy(text, result, 16 * sizeof(uint8_t));
}

std::string AESEncrypt(const std::string &plaintext, const std::string &key) {
    // 对密钥进行哈希
    std::string hashedKey = sha256(key);

    // 初始化加密上下文
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw std::runtime_error("Failed to create EVP_CIPHER_CTX");
    }

    const EVP_CIPHER *cipher = EVP_aes_256_cbc();

    // 生成随机 IV
    unsigned char iv[EVP_MAX_IV_LENGTH];
    if (RAND_bytes(iv, EVP_MAX_IV_LENGTH) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to generate random IV");
    }

    // 初始化加密操作
    if (EVP_EncryptInit_ex(ctx, cipher, nullptr, (const unsigned char *)hashedKey.c_str(), iv) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to initialize encryption");
    }

    // 执行加密
    std::vector<unsigned char> ciphertext(plaintext.size() + EVP_CIPHER_block_size(cipher));
    int len;
    if (EVP_EncryptUpdate(ctx, ciphertext.data(), &len, (const unsigned char *)plaintext.c_str(), plaintext.size()) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to encrypt data");
    }

    int ciphertext_len = len;
    if (EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to finalize encryption");
    }
    ciphertext_len += len;

    // 释放加密上下文
    EVP_CIPHER_CTX_free(ctx);

    // 将 IV 和加密后的密文组合
    std::vector<unsigned char> ciphertext_with_iv(EVP_MAX_IV_LENGTH + ciphertext_len);
    memcpy(ciphertext_with_iv.data(), iv, EVP_MAX_IV_LENGTH);
    memcpy(ciphertext_with_iv.data() + EVP_MAX_IV_LENGTH, ciphertext.data(), ciphertext_len);

    // 将加密结果转换为 16 进制字符串
    return toHex(ciphertext_with_iv.data(), ciphertext_with_iv.size());
}

std::string AESDecrypt(const std::string &hexCiphertext, const std::string &key) {
    // 对密钥进行哈希
    std::string hashedKey = sha256(key);

    // 将 16 进制字符串转换为二进制数据
    std::vector<unsigned char> ciphertext = fromHex(hexCiphertext);

    // 初始化解密上下文
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw std::runtime_error("Failed to create EVP_CIPHER_CTX");
    }

    const EVP_CIPHER *cipher = EVP_aes_256_cbc();

    // 提取 IV 和密文
    const unsigned char *iv = ciphertext.data();
    const unsigned char *encryptedText = ciphertext.data() + EVP_MAX_IV_LENGTH;
    int encryptedText_len = ciphertext.size() - EVP_MAX_IV_LENGTH;

    // 初始化解密操作
    if (EVP_DecryptInit_ex(ctx, cipher, nullptr, (const unsigned char *)hashedKey.c_str(), iv) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to initialize decryption");
    }

    // 执行解密
    std::vector<unsigned char> plaintext(encryptedText_len + EVP_CIPHER_block_size(cipher));
    int len;
    if (EVP_DecryptUpdate(ctx, plaintext.data(), &len, encryptedText, encryptedText_len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to decrypt data");
    }

    int plaintext_len = len;
    if (EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to finalize decryption");
    }
    plaintext_len += len;

    // 释放解密上下文
    EVP_CIPHER_CTX_free(ctx);

    return std::string((char *)plaintext.data(), plaintext_len);
}

std::string toHex(const unsigned char* data, size_t len) {
    static const char hex_digits[] = "0123456789ABCDEF";
    std::string hexStr;
    hexStr.reserve(len * 2); // 每个字节对应两个16进制字符
    for (size_t i = 0; i < len; ++i) {
        hexStr.push_back(hex_digits[data[i] >> 4]);
        hexStr.push_back(hex_digits[data[i] & 0x0F]);
    }
    return hexStr;
}

std::vector<unsigned char> fromHex(const std::string& hexStr) {
    std::vector<unsigned char> data;
    data.reserve(hexStr.size() / 2); // 每两个字符对应一个字节

    for (size_t i = 0; i < hexStr.length(); i += 2) {
        unsigned char byte = (unsigned char) (std::stoi(hexStr.substr(i, 2), nullptr, 16));
        data.push_back(byte);
    }

    return data;
}