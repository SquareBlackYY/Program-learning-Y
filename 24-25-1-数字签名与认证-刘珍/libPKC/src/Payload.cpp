//
// Created by 张笑宇 on 2024/10/12.
//

#include "Payload.h"

#include <ostream>
#include <utility>

using namespace std;

Payload::Payload() = default;

Payload::Payload(string tag) : tag(std::move(tag)) {
}

Payload::Payload(string tag, map<string, string> contents) :tag(std::move(tag)), contents(std::move(contents)) {
}

ostream& operator<<(ostream& os, const Payload& payload) {
    os << " - " << payload.tag << " :" << endl;
    for (const auto& pair : payload.contents) {
        os << "     - " << pair.first << " : " << pair.second << endl;
    }
    return os;
}