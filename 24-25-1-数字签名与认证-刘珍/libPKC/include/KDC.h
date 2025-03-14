//
// Created by 张笑宇 on 2024/10/12.
//

#ifndef KDC_H
#define KDC_H

#include "Host.h"

class KDC final : public Host {

public:
    using Host::Host;

    Message NeedhamSchroederProtocol(const Message &msg) override;
};

#endif // KDC_H
