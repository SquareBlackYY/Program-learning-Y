#include "pkc.h"

#include <thread>
using namespace std;

int main() {
    // 创建公共频道
    PublicChannel channel;

    // 创建 User 对象
    User A("Alice", channel);
    User B("Bob", channel);

    // 启动线程，User 轮询消息
    thread userAThread(&User::pollMessages, &A);
    thread userBThread(&User::pollMessages, &B);

    // 模拟发送消息
    Message msg("admin", "Alice", "请求协商共享密钥", {(Payload){"协议信息", {{"协议", "STS协议"}, {"步骤", "0"}, {"目标", "Bob"}}}});
    channel.sendMessage(msg);
    this_thread::sleep_for(chrono::seconds(1));

    // 打印状态
    cout << A << endl;
    cout << B << endl;

    // 等待线程完成
    userAThread.join();
    userBThread.join();

    return 0;
}