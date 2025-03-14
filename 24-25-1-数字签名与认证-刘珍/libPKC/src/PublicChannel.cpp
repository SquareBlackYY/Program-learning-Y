//
// Created by 张笑宇 on 2024/10/12.
//

#include "PublicChannel.h"

using namespace std;

void PublicChannel::sendMessage(const Message& msg) {
    lock_guard<mutex> lock(mtx);
    messageQueue.push(msg);
    cv.notify_all();  // 通知等待中的线程有新消息
}

Message PublicChannel::receiveMessage(const string &receiverID) {
    unique_lock<mutex> lock(mtx);
    cv.wait(lock, [this, &receiverID]() {
        return !messageQueue.empty() && messageQueue.front().receiver == receiverID;
    });

    Message msg = messageQueue.front();
    messageQueue.pop();
    return msg;
}