//
// Created by 张笑宇 on 2024/10/12.
//

#ifndef USER_H
#define USER_H

#include "Host.h"

class User final : public Host{

public:
    using Host::Host;

    Message NeedhamSchroederProtocol(const Message &msg) override;
};

#endif // USER_H
