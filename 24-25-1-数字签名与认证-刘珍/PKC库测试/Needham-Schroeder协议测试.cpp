#include "pkc.h"

#include <thread>
using namespace std;

int main() {
    // 创建公共频道
    PublicChannel channel;

    // 创建 KDC 和 User 对象
    KDC KDC("KDC", channel);
    User A("Alice", channel);
    User B("Bob", channel);

    // 启动线程，KDC 和 User 轮询消息
    thread kdcThread(&KDC::pollMessages, &KDC);
    thread userAThread(&User::pollMessages, &A);
    thread userBThread(&User::pollMessages, &B);

    // 模拟发送消息
    Message msg1("admin", "Alice", "请求协商共享密钥", {(Payload){"协议信息", {{"协议", "Diffie-Hellman密钥交换"}, {"步骤", "0"}, {"目标", "KDC"}}}});
    Message msg2("admin", "Bob", "请求协商共享密钥", {(Payload){"协议信息", {{"协议", "Diffie-Hellman密钥交换"}, {"步骤", "0"}, {"目标", "KDC"}}}});
    channel.sendMessage(msg1);
    channel.sendMessage(msg2);
    this_thread::sleep_for(chrono::seconds(1));

    // 打印状态
    cout << KDC << endl;
    cout << A << endl;
    cout << B << endl;

    Message msg3("admin", "Alice", "发起双向认证", {(Payload){"协议信息", {{"协议", "Needham-Schroeder双向认证"}, {"步骤", "0"}, {"目标", "Bob"}}}});
    channel.sendMessage(msg3);
    this_thread::sleep_for(chrono::seconds(1));

    cout << KDC << endl;
    cout << A << endl;
    cout << B << endl;

    // 等待线程完成
    kdcThread.join();
    userAThread.join();
    userBThread.join();

    return 0;
}