//
// Created by 张笑宇 on 2024/10/12.
//

#ifndef HOST_H
#define HOST_H

#include <string>
#include <vector>
#include <thread>
#include <iostream>

#include "PublicChannel.h"
#include "Message.h"
#include "Session.h"

class Session;

class Host {
protected:
    std::string ID; // 每个 Host 有一个 ID
    PublicChannel &channel; // 公共频道的引用

    std::vector<std::string> trustList; // 信任列表
    std::map<std::string, std::string> sharedKeyList; // 共享密钥列表
    std::map<std::string, std::unique_ptr<Session>> sessionList; // 会话列表

public:
    virtual ~Host() = default;

    Host(std::string id, PublicChannel &ch);

    // 获取ID
    std::string id() const;

    // 线程轮询公共频道
    void pollMessages();

    // 处理消息
    Message processMessage(const Message &msg);

    // Diffie-Hellman密钥交换
    Message DiffieHellmanProtocol(const Message &msg);

    // Needham-Schroeder双向认证
    virtual Message NeedhamSchroederProtocol(const Message &msg);

    // STS具有密钥协商功能的相互认证协议
    Message STSProtocol(const Message &msg);

    // 添加共享密钥
    void addSharedKey(const std::string& ID, const std::string &key);

    // 获取对应用户的共享密钥
    std::string getSharedKey(const std::string &ID);

    // 添加信任主机
    void addTrustHost(const std::string& ID);

    // 重载 << 运算符
    friend std::ostream& operator<<(std::ostream& os, const Host& host);
};

#endif //HOST_H
