//
// Created by 张笑宇 on 2024/10/14.
//

#include "STS.h"

#include <utility>

#include "AES.h"
#include "Tools.h"

using namespace std;

STS::STS(string id) : targetID(std::move(id)) {
}

Message STS::executeStep(const Message &msg, Host *host) {
    const int messageStep = msg.payloads[0].contents.find("步骤")->second[0] - '0';
    if (messageStep <= step) {
        return {};
    }

    Message response(msg.receiver, targetID);
    response.payloads.emplace_back("协议");
    response.payloads[0].contents["协议"] = "STS协议";
    response.description = "STS协议过程";

    switch (messageStep) {
        // A
        case 0: {
            response.payloads[0].contents["步骤"] = "1";
            step = 1;

            g = "14777377999967849666226757901157577468444643860798182661856789517423526464749";
            p = "57306633641169914669451755090303447125850028436783526643107347283714669367683";
            randNum = getRandNum(2, p);
            const mpz_class g_randNum = powm(g, randNum, p);

            response.payloads.emplace_back("参数");
            response.payloads[1].contents["g"] = g.get_str();
            response.payloads[1].contents["p"] = p.get_str();
            response.payloads[1].contents["g_randNum"] = g_randNum.get_str();

            break;
        }
        // B
        case 1: {
            response.payloads[0].contents["步骤"] = "2";
            step = 2;

            g = msg.payloads[1].contents.find("g")->second;
            p = msg.payloads[1].contents.find("p")->second;

            randNum = getRandNum(2, p);
            const mpz_class g_randNum = powm(g, randNum, p);

            received_g_randNum_mod_p = msg.payloads[1].contents.find("g_randNum")->second;

            sharedKey = powm(received_g_randNum_mod_p, randNum, p);
            host->addSharedKey(targetID, sharedKey.get_str());

            elgamalPrivateKey = elgamal.getPrivateKey(getRandNum(1, p - 1));
            elgamalPublicKey = elgamal.getPublicKey(elgamalPrivateKey);

            const ElGamal::Signature sig = elgamalPrivateKey.sign(g_randNum.get_str() + received_g_randNum_mod_p.get_str());

            const string encrypt_text = AESEncrypt(sig.r.get_str() + " " + sig.s.get_str(), sharedKey.get_str());

            response.payloads.emplace_back("参数");
            response.payloads[1].contents["g_randNum"] = g_randNum.get_str();
            response.payloads[1].contents["E_K(S_B(g^y, g^x))"] = encrypt_text;
            response.payloads[1].contents["elgamal_pk_B"] = elgamalPublicKey.y.get_str();

            break;
        }
        // A
        case 2: {
            response.payloads[0].contents["步骤"] = "3";
            step = 3;

            received_g_randNum_mod_p = msg.payloads[1].contents.find("g_randNum")->second;

            sharedKey = powm(received_g_randNum_mod_p, randNum, p);
            host->addSharedKey(targetID, sharedKey.get_str());

            const string received_encrypted_text = msg.payloads[1].contents.find("E_K(S_B(g^y, g^x))")->second;

            const string received_sig_str = AESDecrypt(received_encrypted_text, sharedKey.get_str());

            istringstream iss(received_sig_str);
            ElGamal::Signature received_sig;
            iss >> received_sig.r >> received_sig.s;

            targetElGamalPublicKey = elgamal.setPublicKey(mpz_class(msg.payloads[1].contents.find("elgamal_pk_B")->second));

            if(targetElGamalPublicKey.verify(received_sig, received_g_randNum_mod_p.get_str() + powm(g, randNum, p).get_str())) {
                host->addTrustHost(targetID);

                elgamalPrivateKey = elgamal.getPrivateKey(getRandNum(1, p - 1));
                elgamalPublicKey = elgamal.getPublicKey(elgamalPrivateKey);

                const ElGamal::Signature sig = elgamalPrivateKey.sign(powm(g, randNum, p).get_str() + received_g_randNum_mod_p.get_str());

                const string encrypt_text = AESEncrypt(sig.r.get_str() + " " + sig.s.get_str(), sharedKey.get_str());

                response.payloads.emplace_back("参数");
                response.payloads[1].contents["E_K(S_A(g^x, g^y))"] = encrypt_text;
                response.payloads[1].contents["elgamal_pk_A"] = elgamalPublicKey.y.get_str();
            } else {
                return {};
            }

            break;
        }
        // B
        case 3: {
            const string received_encrypted_text = msg.payloads[1].contents.find("E_K(S_A(g^x, g^y))")->second;

            const string received_sig_str = AESDecrypt(received_encrypted_text, sharedKey.get_str());

            istringstream iss(received_sig_str);
            ElGamal::Signature received_sig;
            iss >> received_sig.r >> received_sig.s;

            targetElGamalPublicKey = elgamal.setPublicKey(mpz_class(msg.payloads[1].contents.find("elgamal_pk_A")->second));

            if(targetElGamalPublicKey.verify(received_sig, received_g_randNum_mod_p.get_str() + powm(g, randNum, p).get_str())) {
                host->addTrustHost(targetID);
                return {};
            } else {
                return {};
            }

            break;
        }
        default: return {};
    }

    return response;
}
