//
// Created by 张笑宇 on 2024/10/12.
//

#include "Host.h"

#include <utility>

#include "DiffieHellman.h"
#include "STS.h"

using namespace std;

Host::Host(string id, PublicChannel &ch) : ID(std::move(id)), channel(ch) {
}

string Host::id() const { return ID; }

void Host::pollMessages() {
    while (true) {
        Message msg = channel.receiveMessage(ID);
        cout << "[" << ID << "] 收到来自 " << msg.sender << " 的消息：" << msg << endl;
        // 处理消息并产生响应
        Message response = processMessage(msg);
        if (!response.payloads.empty()) { channel.sendMessage(response); }
    }
}

Message Host::processMessage(const Message &msg) {
    Message response;
    // 获取包中协议名
    const auto protocol = msg.payloads[0].contents.find("协议")->second;

    if (protocol == "Diffie-Hellman密钥交换") {
        response = DiffieHellmanProtocol(msg);
    } else if (protocol == "Needham-Schroeder双向认证") {
        response = NeedhamSchroederProtocol(msg);
    } else if (protocol == "STS协议") {
        response = STSProtocol(msg);
    } else {
        return {};
    }

    return response;
}

Message Host::DiffieHellmanProtocol(const Message &msg) {
    const auto protocol = msg.payloads[0].contents.find("协议")->second;
    // 整合出会话ID
    const auto sessionID = msg.sender + "_" + protocol;
    // 获取包中协议步骤
    const auto step = msg.payloads[0].contents.find("步骤");
    // 在自身会话列表中查找对应会话
    const auto session = sessionList.find(sessionID);

    // 共享密钥列表中没有与对应用户的共享密钥
    if (sharedKeyList.find(msg.sender) == sharedKeyList.end()) {
        // 会话列表中没有与指定用户进行的协议
        if (session == sessionList.end()) {
            if (step->second == "0") {
                const auto target = msg.payloads[0].contents.find("目标")->second;
                sessionList[target + "_" + protocol] = unique_ptr<Session>(new DiffieHellman(target));
                return sessionList[target + "_" + protocol]->executeStep(msg, this);
            }
            if (step->second == "1") {
                // 创建对应用户对应协议的对象
                sessionList[sessionID] = unique_ptr<Session>(new DiffieHellman(msg.sender));
                // 传入消息，执行协议步骤
                Message response = sessionList[sessionID]->executeStep(msg, this);

                sessionList.erase(sessionID);
                return response;
            }
        }
        // 会话列表中存在会话 且 处于协议执行阶段
        else if (session != sessionList.end() && step->second == "2") {
            Message response = sessionList[sessionID]->executeStep(msg, this);

            sessionList.erase(sessionID);
            return response;
        }
    }

    return {};
}

Message Host::NeedhamSchroederProtocol(const Message &msg) {
    return {};
}

Message Host::STSProtocol(const Message &msg) {
    const auto protocol = msg.payloads[0].contents.find("协议")->second;
    // 整合出会话ID
    const auto sessionID = msg.sender + "_" + protocol;
    // 获取包中协议步骤
    const auto step = msg.payloads[0].contents.find("步骤")->second;
    // 在自身会话列表中查找对应会话
    const auto session = sessionList.find(sessionID);

    // 会话列表中没有与指定用户进行的协议
    if (session == sessionList.end()) {
        if (step == "0") {
            const auto target = msg.payloads[0].contents.find("目标")->second;
            sessionList[target + "_" + protocol] = unique_ptr<Session>(new STS(target));
            return sessionList[target + "_" + protocol]->executeStep(msg, this);
        }
        if (step == "1") {
            // 创建对应用户对应协议的对象
            sessionList[sessionID] = unique_ptr<Session>(new STS(msg.sender));
            // 传入消息，执行协议步骤
            Message response = sessionList[sessionID]->executeStep(msg, this);

            return response;
        }
    }
    // 会话列表中存在会话 且 处于协议执行阶段
    else if (session != sessionList.end() && (step == "2" || step == "3")) {
        Message response = sessionList[sessionID]->executeStep(msg, this);

        sessionList.erase(sessionID);
        return response;
    }


    return {};
}

void Host::addSharedKey(const string &ID, const string &key) {
    if (sharedKeyList.find(ID) == sharedKeyList.end()) {
        sharedKeyList[ID] = key;
    }
}

string Host::getSharedKey(const string &ID) {
    return sharedKeyList.find(ID)->second;
}

void Host::addTrustHost(const std::string &ID) {
    trustList.push_back(ID);
}

ostream &operator<<(std::ostream &os, const Host &host) {
    os << "主机 [" << host.ID << "]" << endl;
    os << " - 信任列表:" << endl;
    for (const auto &str: host.trustList) {
        cout << "     - " << str << endl;
    }

    os << " - 共享密钥列表:" << endl;
    for (const auto &pair: host.sharedKeyList) {
        cout << "     - [" << pair.first << "] : " << pair.second << endl;
    }

    return os;
}
