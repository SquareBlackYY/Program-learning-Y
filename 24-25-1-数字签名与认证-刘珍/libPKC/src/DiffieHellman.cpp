//
// Created by 张笑宇 on 2024/10/12.
//

#include "DiffieHellman.h"

using namespace std;

DiffieHellman::DiffieHellman(string id) : targetUserID(std::move(id)) {
}

Message DiffieHellman::executeStep(const Message &msg, Host *host) {
    const int messageStep = msg.payloads[0].contents.find("步骤")->second[0] - '0';
    if (messageStep <= step) {
        return {};
    }

    Message response(msg.receiver, targetUserID);
    response.payloads.emplace_back("协议");
    response.payloads[0].contents["协议"] = "Diffie-Hellman密钥交换";
    response.description = "Diffie-Hellman密钥协商过程";

    switch (messageStep) {
        case 0: {
            response.payloads[0].contents["步骤"] = "1";
            step = 1;

            g = "14777377999967849666226757901157577468444643860798182661856789517423526464749";
            p = "57306633641169914669451755090303447125850028436783526643107347283714669367683";
            randomNum = getRandNum(2, p);
            const mpz_class g_randomNum = powm(g, randomNum, p);

            response.payloads.emplace_back("参数");
            response.payloads[1].contents["g"] = g.get_str();
            response.payloads[1].contents["p"] = p.get_str();
            response.payloads[1].contents["g_randomNum"] = g_randomNum.get_str();

            break;
        }
        case 1: {
            response.payloads[0].contents["步骤"] = "2";
            step = 2;

            g = msg.payloads[1].contents.find("g")->second;
            p = msg.payloads[1].contents.find("p")->second;

            randomNum = getRandNum(2, p);
            const mpz_class g_randomNum = powm(g, randomNum, p);

            const mpz_class receive_g_randomNum(msg.payloads[1].contents.find("g_randomNum")->second);

            sharedKey = powm(receive_g_randomNum, randomNum, p);
            host->addSharedKey(targetUserID, sharedKey.get_str());

            response.payloads.emplace_back("参数");
            response.payloads[1].contents["g_randomNum"] = g_randomNum.get_str();

            break;
        }
        case 2: {
            const mpz_class receive_g_randomNum(msg.payloads[1].contents.find("g_randomNum")->second);

            sharedKey = powm(receive_g_randomNum, randomNum, p);
            host->addSharedKey(targetUserID, sharedKey.get_str());

            return {};
        }
        default: return {};
    }

    return response;
}
