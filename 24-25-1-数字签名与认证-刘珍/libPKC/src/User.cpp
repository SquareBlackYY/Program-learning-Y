//
// Created by 张笑宇 on 2024/10/12.
//

#include "User.h"
#include "NeedhamSchroeder.h"

using namespace std;

Message User::NeedhamSchroederProtocol(const Message &msg) {
    const auto protocol = msg.payloads[0].contents.find("协议")->second;
    // 整合出会话ID
    const auto sessionID = msg.sender + "_" + protocol;
    // 获取包中协议步骤
    const auto step = msg.payloads[0].contents.find("步骤")->second;
    // 在自身会话列表中查找对应会话
    const auto session = sessionList.find(sessionID);

    if(step == "0") {
        const string target = "KDC";
        sessionList[target + "_" + protocol] = unique_ptr<Session>(new NeedhamSchroeder());
        return sessionList[target + "_" + protocol]->executeStep(msg, this);
    }
    else if(step == "2") {
        const string target = "Bob";
        sessionList[target + "_" + protocol] = std::move(sessionList["KDC_" + protocol]);
        sessionList.erase("KDC_" + protocol);
        return sessionList[target + "_" + protocol]->executeStep(msg, this);
    }
    else if(step == "3") {
        sessionList[sessionID] = unique_ptr<Session>(new NeedhamSchroeder());
        return sessionList[sessionID]->executeStep(msg, this);
    }
    else if(step == "4") {
        Message response = sessionList[sessionID]->executeStep(msg, this);

        sessionList.erase(sessionID);
        return response;
    }
    else if(step == "5") {
        Message response = sessionList[sessionID]->executeStep(msg, this);

        sessionList.erase(sessionID);
        return response;
    }

    return {};
}
