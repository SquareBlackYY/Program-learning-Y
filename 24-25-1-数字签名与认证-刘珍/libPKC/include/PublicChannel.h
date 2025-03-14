//
// Created by 张笑宇 on 2024/10/12.
//

#ifndef PUBLICCHANNEL_H
#define PUBLICCHANNEL_H

#include <vector>
#include <map>
#include <string>
#include <mutex>
#include <condition_variable>
#include <queue>

#include "Message.h"

class PublicChannel {
    std::queue<Message> messageQueue;   // 公共频道中的消息队列
    std::mutex mtx;                     // 保护消息队列的互斥锁
    std::condition_variable cv;         // 用于通知线程消息到达

public:
    // 发送消息到公共频道
    void sendMessage(const Message& msg);

    // 从公共频道中接收消息
    Message receiveMessage(const std::string &receiverID);
};

#endif // PUBLICCHANNEL_H
