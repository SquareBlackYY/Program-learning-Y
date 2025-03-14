//
// Created by 张笑宇 on 2024/10/12.
//

#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <vector>
#include "Payload.h"

class Message {
public:
    std::string sender;
    std::string receiver;
    std::string description;
    std::vector<Payload> payloads;

    Message();

    Message(std::string from, std::string to);

    Message(std::string from, std::string to, std::string description);

    Message(std::string from, std::string to, std::string description, std::vector<Payload> payloads);

    // 重载 << 运算符
    friend std::ostream& operator<<(std::ostream& os, const Message& message);
};

#endif // MESSAGE_H
