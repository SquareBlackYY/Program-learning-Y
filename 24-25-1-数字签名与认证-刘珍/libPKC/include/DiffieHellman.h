//
// Created by 张笑宇 on 2024/10/12.
//

#ifndef DIFFIEHELLMAN_H
#define DIFFIEHELLMAN_H

#include <gmpxx.h>
#include <iostream>
#include <utility>

#include "Tools.h"
#include "Host.h"
#include "Message.h"
#include "Session.h"

// Diffie-Hellman 协议类
class DiffieHellman final : public Session {
    std::string targetUserID;
    int step = -1;

    mpz_class g;
    mpz_class p;
    mpz_class randomNum;
    mpz_class sharedKey;

public:
    explicit DiffieHellman(std::string id);

    Message executeStep(const Message &msg, Host *host) override;

};

#endif //DIFFIEHELLMAN_H
