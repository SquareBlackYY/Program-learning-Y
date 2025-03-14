//
// Created by 张笑宇 on 2024/10/13.
//

#ifndef NEEDHAMSCHROEDER_H
#define NEEDHAMSCHROEDER_H

#include <gmpxx.h>

#include "Session.h"

class NeedhamSchroeder final : public Session {
    std::string targetID_A;
    std::string targetID_B;
    std::string targetID_KDC = "KDC";
    int step = -1;

    mpz_class N_1;
    mpz_class N_2;
    std::string K_S;

    public:
    NeedhamSchroeder();

    Message executeStep(const Message &msg, Host *host) override;
};

#endif //NEEDHAMSCHROEDER_H
