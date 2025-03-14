//
// Created by 张笑宇 on 2024/10/12.
//

#include "Message.h"

#include <ostream>
#include <utility>

using namespace std;

Message::Message() = default;

Message::Message(string from, string to) : sender(std::move(from)), receiver(std::move(to)) {
}

Message::Message(string from, string to, string description) : sender(std::move(from)), receiver(std::move(to)), description(std::move(description)) {
}

Message::Message(string from, string to, string description, vector<Payload> payloads) : sender(std::move(from)), receiver(std::move(to)), description(std::move(description)), payloads(std::move(payloads)) {
}

ostream& operator<<(ostream& os, const Message& message) {
    os << message.description << endl;
    for (const auto& payload : message.payloads) {
        os << payload;
    }
    return os;
}