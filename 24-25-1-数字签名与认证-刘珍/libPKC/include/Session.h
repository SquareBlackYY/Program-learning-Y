//
// Created by 张笑宇 on 2024/10/12.
//

#ifndef SESSION_H
#define SESSION_H

#include "Host.h"
#include "Message.h"

class Host;

// 通用的Session基类
class Session {
public:
    virtual ~Session() = default;
    virtual Message executeStep(const Message &msg, Host *host) = 0;  // 执行协议的下一步
    virtual Message executeStep(const Message &msg, Host *host, const std::string &KDC) {
        return {};
    }
};

#endif //SESSION_H
