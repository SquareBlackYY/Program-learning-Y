from random import randint, choice, triangular

class TransportLayer:
    def __init__(self, window_size):
        self.send_window = window_size                  # 发送窗口大小
        self.receive_window = window_size               # 接收窗口大小
        self.front = window_size + 1                    # 窗口前沿（右边）
        self.mid = 0                                    # 中间指示已发送但未确认的位置
        self.back = 0                                   # 窗口后沿（左边）
        self.send_buffer = [None] * window_size         # 发送缓冲区
        self.receive_buffer = [None] * window_size      # 接收缓冲区
        self.received = [False] * window_size           # 标记接收缓冲区是否接收到数据

    def send(self, datas):
        if self.mid == self.front:
            print("窗口已满，等待 ACK")
            return
        for data in datas:
            self.send_buffer[self.mid - self.back] = data
            print(f"发送数据：{data} 序列号：{self.mid}")
            self.mid += 1

    # 返回 ack
    def receive(self, datas):
        flag = False
        for i in range(len(datas)):
            if not self.receive_buffer[i]:
                select = choice([True, False])
                if select:
                    flag = True
                    self.receive_buffer[i] = datas[i]
                    self.received[i] = True
                    print(f"接收数据：{datas[i]}")

        if not flag:
            print("没有接收到任何数据!")
            return self.back
        for i in range(self.receive_window):
            if not self.received[i]:
                return self.back + i
        return self.front
    
    def submit(self, ack):
        if self.back == ack:
            print(f"没有可以提交的数据!")
        for i in range(self.back, ack):
            print(f"向上提交数据：{self.receive_buffer[0]}")
            self.receive_buffer.pop(0)
            self.received.pop(0)
            self.receive_buffer.append(None)
            self.received.append(False)
        
        rwnd = round(triangular(self.receive_window - 2, self.receive_window + 2, self.receive_window))  # 三角分布(下界，上界，峰值)
        self.receive_buffer = self.receive_buffer[:rwnd] if rwnd < self.receive_window else self.receive_buffer + [None] * (rwnd - self.receive_window)
        self.received = self.received[:rwnd] if rwnd < self.receive_window else self.received + [False] * (rwnd - self.receive_window)
        self.receive_window = rwnd
        print(f"接收窗口大小变为：{self.receive_window}")
        return rwnd

    def slide_window(self, ack, rwnd):
        for i in range(self.back, ack):
            self.send_buffer.pop(0)
            self.send_buffer.append(None)
            self.back += 1
            self.front += 1
        
        self.mid = self.back
        self.send_buffer = self.send_buffer[:rwnd] if rwnd < self.send_window else self.send_buffer + [None] * (rwnd - self.send_window)
        self.send_window = rwnd
        print(f"发送窗口大小变为：{self.send_window}")


R = 10              # 交互次数
window_size = 10    # 初始窗口大小
transport_layer = TransportLayer(window_size)

for i in range(R):
    print(f"第{i + 1}次交互".center(35, '-'))
    datas = list(range(transport_layer.back, transport_layer.back + randint(1, transport_layer.send_window)))
    transport_layer.send(datas)
    ack = transport_layer.receive(datas)
    rwnd = transport_layer.submit(ack)
    transport_layer.slide_window(ack, rwnd)
    print(transport_layer.send_buffer)
    print(transport_layer.receive_buffer)