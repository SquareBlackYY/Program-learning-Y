//
// Created by 张笑宇 on 2024/10/12.
//

#include "KDC.h"

#include "NeedhamSchroeder.h"

using namespace std;

Message KDC::NeedhamSchroederProtocol(const Message &msg) {
    const auto protocol = msg.payloads[0].contents.find("协议")->second;
    // 整合出会话ID
    const auto sessionID = msg.sender + "_" + protocol;
    // 获取包中协议步骤
    const auto step = msg.payloads[0].contents.find("步骤");
    // 在自身会话列表中查找对应会话
    const auto session = sessionList.find(sessionID);

    if(step->second == "1") {
        sessionList[sessionID] = unique_ptr<Session>(new NeedhamSchroeder());
        Message response = sessionList[sessionID]->executeStep(msg, this);

        sessionList.erase(sessionID);
        return response;
    }

    return {};
}