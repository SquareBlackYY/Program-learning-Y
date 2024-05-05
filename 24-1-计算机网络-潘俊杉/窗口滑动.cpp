#include <iostream>
#include <vector>
#include <random>

class TransportLayer
{
public:
    int send_window;                 // 发送窗口大小
    int receive_window;              // 接收窗口大小
    int front;                       // 窗口前沿（右边）
    int mid;                         // 中间指示已发送但未确认的位置
    int back;                        // 窗口后沿（左边）
    std::vector<int> send_buffer;    // 发送缓冲区
    std::vector<int> receive_buffer; // 接收缓冲区
    std::vector<bool> received;      // 标记接收缓冲区是否接收到数据


    TransportLayer(int window_size)
    {
        send_window = window_size;
        receive_window = window_size;
        front = window_size + 1;
        mid = 0;
        back = 0;
        send_buffer.resize(window_size);
        receive_buffer.resize(window_size);
        received.resize(window_size, false);
    }

    void send(std::vector<int> datas)
    {
        if (mid == front)
        {
            std::cout << "窗口已满，等待 ACK" << std::endl;
            return;
        }
        for (int data : datas)
        {
            send_buffer[mid - back] = data;
            std::cout << "发送数据：" << data << " 序列号：" << mid << std::endl;
            mid++;
        }
    }

    int receive(std::vector<int> datas)
    {
        bool flag = false;
        for (int i = 0; i < datas.size(); ++i)
        {
            if (!receive_buffer[i])
            {
                bool select = rand() % 2 == 0; // choice([True, False])
                if (select)
                {
                    flag = true;
                    receive_buffer[i] = datas[i];
                    received[i] = true;
                    std::cout << "接收数据：" << datas[i] << std::endl;
                }
            }
        }
        if (!flag)
        {
            std::cout << "没有接收到任何数据!" << std::endl;
            return back;
        }
        for (int i = 0; i < receive_window; ++i)
        {
            if (!received[i])
            {
                return back + i;
            }
        }
        return front;
    }

    int submit(int ack)
    {
        if (back == ack)
        {
            std::cout << "没有可以提交的数据!" << std::endl;
        }
        for (int i = back; i < ack; ++i)
        {
            std::cout << "向上提交数据：" << receive_buffer[0] << std::endl;
            receive_buffer.erase(receive_buffer.begin());
            received.erase(received.begin());
            receive_buffer.push_back(0);
            received.push_back(false);
        }
        int rwnd = round((receive_window - 2 + receive_window + 2 + receive_window) / 3.0);
        receive_buffer.resize(rwnd < receive_window ? rwnd : receive_window, 0);
        received.resize(rwnd < receive_window ? rwnd : receive_window, false);
        receive_window = rwnd;
        std::cout << "接收窗口大小变为：" << receive_window << std::endl;
        return rwnd;
    }

    void slide_window(int ack, int rwnd)
    {
        for (int i = back; i < ack; ++i)
        {
            send_buffer.erase(send_buffer.begin());
            send_buffer.push_back(0);
            back++;
            front++;
        }
        mid = back;
        send_buffer.resize(rwnd < send_window ? rwnd : send_window, 0);
        send_window = rwnd;
        std::cout << "发送窗口大小变为：" << send_window << std::endl;
    }
};

int main()
{
    const int R = 10;           // 交互次数
    const int window_size = 10; // 初始窗口大小
    TransportLayer transport_layer(window_size);

    for (int i = 0; i < R; ++i)
    {
        std::cout << std::string(35, '-') << std::endl;
        std::cout << "第" << i + 1 << "次交互" << std::endl;
        std::vector<int> datas;
        int start = transport_layer.back;
        int end = transport_layer.back + (rand() % transport_layer.send_window) + 1;
        for (int j = start; j < end; ++j)
            datas.push_back(j);
        transport_layer.send(datas);
        int ack = transport_layer.receive(datas);
        int rwnd = transport_layer.submit(ack);
        transport_layer.slide_window(ack, rwnd);
        for (int num : transport_layer.send_buffer)
            std::cout << num << " ";
        std::cout << std::endl;
        for (int num : transport_layer.receive_buffer)
            std::cout << num << " ";
        std::cout << std::endl;
    }

    return 0;
}
