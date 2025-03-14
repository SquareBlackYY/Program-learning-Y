//
// Created by 张笑宇 on 2024/10/12.
//

#ifndef PAYLOAD_H
#define PAYLOAD_H

#include <map>
#include <string>
#include <vector>

class Payload {
    public:
    std::string tag;
    std::map<std::string, std::string> contents;

    Payload();

    explicit Payload(std::string tag);

    explicit Payload(std::string tag, std::map<std::string, std::string> contents);

    // 重载 << 运算符
    friend std::ostream& operator<<(std::ostream& os, const Payload& payload);
};

#endif //PAYLOAD_H
