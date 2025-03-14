//
// Created by 张笑宇 on 2024/10/13.
//

#include "NeedhamSchroeder.h"

#include "AES.h"
#include "Tools.h"

using namespace std;

NeedhamSchroeder::NeedhamSchroeder() = default;

Message NeedhamSchroeder::executeStep(const Message &msg, Host *host) {
    const int messageStep = msg.payloads[0].contents.find("步骤")->second[0] - '0';
    if (messageStep <= step) {
        return {};
    }

    Message response;
    response.payloads.emplace_back("协议");
    response.payloads[0].contents["协议"] = "Needham-Schroeder双向认证";
    response.description = "Needham-Schroeder双向认证过程";

    switch (messageStep) {
        // A
        case 0: {
            response.payloads[0].contents["步骤"] = "1";
            step = 1;

            targetID_A = msg.receiver;
            targetID_B = msg.payloads[0].contents.find("目标")->second;

            response.sender = targetID_A;
            response.receiver = targetID_KDC;

            response.payloads.emplace_back("参数");
            response.payloads[1].contents["ID_A"] = targetID_A;
            response.payloads[1].contents["ID_B"] = targetID_B;
            N_1 = getRandNum(1, INT32_MAX);
            response.payloads[1].contents["N_1"] = N_1.get_str();

            break;
        }
        // KDC
        case 1: {
            response.payloads[0].contents["步骤"] = "2";
            step = 2;

            targetID_A = msg.sender;
            targetID_B = msg.payloads[1].contents.find("ID_B")->second;

            N_1 = msg.payloads[1].contents.find("N_1")->second;

            response.sender = targetID_KDC;
            response.receiver = targetID_A;

            K_S = getRandNum(1, INT32_MAX).get_str();

            string cipherText_K_B = AESEncrypt(K_S + " " + targetID_A, host->getSharedKey(targetID_B));
            string cipherText_K_A = AESEncrypt(K_S + " " + targetID_B + " " + N_1.get_str() + " " + cipherText_K_B, host->getSharedKey(targetID_A));

            response.payloads.emplace_back("参数");
            response.payloads[1].contents["E_K_A(K_S, ID_B, N_1, E_K_B(K_S, ID_A))"] = cipherText_K_A;
            break;
        }
        // A
        case 2: {
            response.payloads[0].contents["步骤"] = "3";
            step = 3;

            response.sender = targetID_A;
            response.receiver = targetID_B;

            string encryptedText = msg.payloads[1].contents.find("E_K_A(K_S, ID_B, N_1, E_K_B(K_S, ID_A))")->second;
            string decryptedText = AESDecrypt(encryptedText, host->getSharedKey(targetID_KDC));

            istringstream iss(decryptedText);
            string E_K_B, temp;
            iss >> K_S >> temp >> temp >> E_K_B;

            host->addSharedKey(targetID_B, K_S);
            host->addTrustHost(targetID_B);

            response.payloads.emplace_back("参数");
            response.payloads[1].contents["E_K_B(K_S, ID_A)"] = E_K_B;

            break;
        }
        // B
        case 3: {
            response.payloads[0].contents["步骤"] = "4";
            step = 4;

            targetID_A = msg.sender;
            targetID_B = msg.receiver;

            response.sender = targetID_B;
            response.receiver = targetID_A;

            string encryptedText = msg.payloads[1].contents.find("E_K_B(K_S, ID_A)")->second;
            string decryptedText = AESDecrypt(encryptedText, host->getSharedKey(targetID_KDC));

            istringstream iss(decryptedText);
            string temp;
            iss >> K_S >> temp;

            N_2 = getRandNum(1, INT32_MAX);
            string E_K_S = AESEncrypt(N_2.get_str(), K_S);

            response.payloads.emplace_back("参数");
            response.payloads[1].contents["E_K_S(N_2)"] = E_K_S;

            break;
        }
        // A
        case 4: {
            response.payloads[0].contents["步骤"] = "5";
            step = 5;

            response.sender = targetID_A;
            response.receiver = targetID_B;

            string encryptedText = msg.payloads[1].contents.find("E_K_S(N_2)")->second;
            string decryptedText = AESDecrypt(encryptedText, K_S);

            N_2 = mpz_class(decryptedText);

            string E_K_S = AESEncrypt(static_cast<mpz_class>(N_2 + 1).get_str(), K_S);

            response.payloads.emplace_back("参数");
            response.payloads[1].contents["E_K_S(f(N_2))"] = E_K_S;

            break;
        }
        // B
        case 5: {
            string encryptedText = msg.payloads[1].contents.find("E_K_S(f(N_2))")->second;
            string decryptedText = AESDecrypt(encryptedText, K_S);

            if(N_2 + 1 == mpz_class(decryptedText)) {
                host->addSharedKey(targetID_A, K_S);
                host->addTrustHost(targetID_A);
            }

            return {};
        }
        default: return {};
    }

    return response;
}
