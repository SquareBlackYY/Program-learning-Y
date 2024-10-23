#include <iostream>
#include <queue>
#include <random>
using namespace std;

// 最大容纳的进程数量
constexpr int MAX_NUM = 1000;

// 进程管理器类，用于管理不同进程的调度和状态
class ProcessManager {
public:
    // 进程控制块（Process Control Block）存储进程的基本信息和状态
    struct PCB {
        string process_name;        // 进程名称
        int process_id = 0;         // 进程编号
        int priority_level = 0;     // 进程优先级
        int arrival_time = 0;       // 进程到达时间
        int total_service_time = 0; // 进程所需总时间
        int remaining_time = 0;     // 进程剩余时间
        int cpu_time_used = 0;      // 进程 CPU 已使用时间
        string current_state;       // 进程当前状态
        int completion_time = 0;    // 进程完成时的系统 CPU 时间
        string block_reason;        // 进程阻塞原因

        // 空参构造
        PCB() = default;

        //  通过进程名和进程ID创建随机参数进程对象
        PCB(string name, const int id) : process_name(std::move(name)), process_id(id) {
            priority_level = randomInRange(1, 9);
            arrival_time = randomInRange(0, 9);
            total_service_time = randomInRange(1, 9);
            remaining_time = randomInRange(1, 9);
        }

        // 生成范围内随机数的函数（前闭后闭）
        static int randomInRange(const int min, const int max) {
            // 创建一个随机设备来提供不可预测的种子
            random_device rd;
            // 使用 Mersenne Twister 算法生成随机数
            mt19937 gen(rd());
            // 设置分布范围
            uniform_int_distribution<> distrib(min, max);
            return distrib(gen);
        }
    };

private:
    queue<PCB> process_queue_;         // 进程队列，存储所有进程
    queue<PCB> ready_queue_;           // 就绪队列，存储准备运行的进程
    queue<PCB> block_queue_;           // 阻塞队列，存储处于阻塞状态的进程
    queue<PCB> run_queue_;             // 运行队列，存储当前正在运行的进程
    queue<PCB> finish_queue_;          // 完成队列，存储已完成的进程
    queue<PCB> hung_queue_;            // 挂起队列，存储挂起的进程
    int total_process_id_ = 1;         // 全局进程 ID 计数器
    int system_cpu_time_ = 0;          // 当前系统 CPU 时间
    int scheduling_algorithm_ = 0;     // 当前使用的调度算法
    int time_slice_ = 0;               // 时间片，用于时间片轮转调度算法
    bool scheduling_finished_ = false; // 进程调度是否结束

    // 升序排列 PCB 对象的到达时间
    struct SortByArrivalTime {
        bool operator()(const PCB &p1, const PCB &p2) const {
            return p1.arrival_time < p2.arrival_time;
        }
    };

    // 降序排列 PCB 对象的优先级
    struct SortByPriorityDescending {
        bool operator()(const PCB &p1, const PCB &p2) const {
            return p1.priority_level > p2.priority_level;
        }
    };

public:
    // 自动创建随机进程
    void createRandomProcesses()
    {
        PCB p1("a", total_process_id_++);
        process_queue_.push(p1);

        PCB p2("b", total_process_id_++);
        process_queue_.push(p2);

        PCB p3("c", total_process_id_++);
        process_queue_.push(p3);

        PCB p4("d", total_process_id_++);
        process_queue_.push(p4);

        PCB p5("e", total_process_id_++);
        process_queue_.push(p5);
    }

    // 手动创建进程
    void createProcessByUser()
    {
        int n;
        cout << "请输入要添加的进程数：" << endl;
        cin >> n;

        for (int i = 0; i < n; i++) {
            PCB p;
            cout << "请输入第" << i + 1 << "个进程名称、进程优先级、进程到达时间、进程需要运行时间:" << endl;
            cin >> p.process_name >> p.priority_level >> p.arrival_time >> p.total_service_time;

            // 进程编号自增
            p.process_id = total_process_id_++;
            // 新添加进程的到达时间要加上当前的系统的 CPU 时间
            p.arrival_time += system_cpu_time_;
            // 剩余时间为所需运行时间
            p.remaining_time = p.total_service_time;

            process_queue_.push(p);
        }
    }

    // 系统开始调度
    void beginDispatch() {
        // 如果运行队列为空，尝试从其他队列中调度进程
        if (run_queue_.empty()) {
            // 如果就绪队列不为空，则将第一个就绪进程移动到运行队列
            if (!ready_queue_.empty()) {
                // 更新进程状态为"运行"
                ready_queue_.front().current_state = "Run";
                // 将进程从就绪队列推到运行队列
                run_queue_.push(ready_queue_.front());
                // 从就绪队列移除该进程
                ready_queue_.pop();
            }
            // 如果就绪队列为空但阻塞队列或挂起队列有进程，则处理这些队列
            else if (!block_queue_.empty() || !hung_queue_.empty()) {
                handleEmptyQueues();
            }
            // 如果所有队列都为空，表示调度已完成
            else {
                scheduling_finished_ = true;
            }
        }
        // 如果运行队列不为空，处理运行中的进程
        else if (!run_queue_.empty()) {
            processRunQueue();
        }
    }

    // 时间片到期处理
    void handleTimeSliceExpiration() {
        if (!run_queue_.empty()) {
            if (run_queue_.front().remaining_time != 0) {
                // 时间片未结束，将当前运行进程移到就绪队列队尾
                PCB p = run_queue_.front();
                run_queue_.pop();
                p.current_state = "Ready";
                // 先来先服务算法：更新到达时间
                if (scheduling_algorithm_ == 1) {
                    p.arrival_time = ready_queue_.back().arrival_time + 1;
                }
                // 将进程压入就绪队列
                ready_queue_.push(p);
            }
        }
        else {
            cout << "当前无运行进程" << endl;
        }
    }

    // 处理其他阻塞事件
    void handleOtherBlockingEvents() {
        if (!run_queue_.empty()) {
            if (run_queue_.front().remaining_time != 0) {
                // 将当前运行进程移到阻塞队列，并标记为等待状态
                run_queue_.front().block_reason = "其他阻塞事件";
                run_queue_.front().current_state = "Wait";
                block_queue_.push(run_queue_.front());
                run_queue_.pop();
            }
        }
        else {
            cout << "当前无运行进程" << endl;
        }
    }

    // 处理其他阻塞事件完成
    void handleOtherBlockingEventsCompletion() {
        if (block_queue_.empty()) {
            cout << "目前无阻塞进程" << endl;
        }
        else {
            int id;
            cout << "请输入其他阻塞事件完成的进程ID" << endl;
            cin >> id;
            unblockProcess(id);
        }
    }

    // 处理挂起
    void handleSuspendRequest() {
        if (!run_queue_.empty()) {
            if (run_queue_.front().remaining_time != 0) {
                // 将当前运行进程移到挂起队列
                run_queue_.front().block_reason = "请求挂起";
                run_queue_.front().current_state = "Hung";
                hung_queue_.push(run_queue_.front());
                run_queue_.pop();
            }
        }
        else {
            cout << "当前无运行进程" << endl;
        }
    }

    // 挂起完成后处理
    void handleSuspendCompletion() {
        if (hung_queue_.empty()) {
            cout << "目前无挂起进程" << endl;
        }
        else {
            int id;
            cout << "请输入挂起完成的进程ID" << endl;
            cin >> id;
            unHungProcess(id);
        }
    }

    // 选择调度算法
    void selectSchedulingAlgorithm(const int algo) {
        scheduling_algorithm_ = algo;
        if (scheduling_algorithm_ == 1)
            FCFS();  // 先来先服务调度
        else if (scheduling_algorithm_ == 2)
            SJF();  // 短作业优先调度
        else if (scheduling_algorithm_ == 3) {
            // 时间片轮转调度
            if (time_slice_ == 0) {
                cout << "请输入时间片大小：" << endl;
                cin >> time_slice_;
            }
            RR();
        }
    }

    // 先来先服务调度
    void FCFS() {
        // 对所有进程按到达时间排序
        PCB temp[MAX_NUM];
        int n = 0;
        while (!process_queue_.empty()) {
            temp[n++] = process_queue_.front();
            process_queue_.pop();
        }
        while (!ready_queue_.empty()) {
            temp[n++] = ready_queue_.front();
            ready_queue_.pop();
        }
        // 排序并将进程移回就绪队列
        sort(temp, temp + n, SortByArrivalTime());
        for (int i = 0; i < n; i++) {
            temp[i].current_state = "Ready";
            ready_queue_.push(temp[i]);
        }
    }

    // 短作业优先调度
    void SJF() {
        // 按优先级对就绪队列排序
        PCB temp[MAX_NUM];
        int n = 0;
        while (!process_queue_.empty()) {
            temp[n++] = process_queue_.front();
            process_queue_.pop();
        }
        while (!ready_queue_.empty()) {
            temp[n++] = ready_queue_.front();
            ready_queue_.pop();
        }
        // 排序并将进程移回就绪队列
        sort(temp, temp + n, SortByPriorityDescending());
        for (int i = 0; i < n; i++) {
            temp[i].current_state = "Ready";
            ready_queue_.push(temp[i]);
        }
    }

    // 时间片轮转调度
    void RR() {
        // 不改变顺序，将进程队列移到就绪队列
        while (!process_queue_.empty()) {
            PCB p = process_queue_.front();
            process_queue_.pop();
            p.current_state = "Ready";
            ready_queue_.push(p);
        }
    }

    // 处理运行队列中的进程
    void processRunQueue() {
        // 增加系统CPU时间
        system_cpu_time_++;
        // 更新CPU使用时间
        run_queue_.front().cpu_time_used++;
        // 减少剩余时间
        run_queue_.front().remaining_time--;

        // 如果进程完成，结束进程
        if (run_queue_.front().remaining_time == 0) {
            finishCurrentProcess();
        }
        else {
            // 先来先服务或短作业优先，不做时间片轮转
            if (scheduling_algorithm_ == 1 || scheduling_algorithm_ == 2) {
                return;
            }
            // 时间片轮转，检查时间片是否用完
            if (scheduling_algorithm_ == 3) {
                if (run_queue_.front().cpu_time_used != 0 &&
                    run_queue_.front().cpu_time_used % time_slice_ == 0) {
                    // 时间片用完，进程重新进入就绪队列
                    run_queue_.front().current_state = "Ready";
                    ready_queue_.push(run_queue_.front());
                    run_queue_.pop();
                }
            }
        }

        // 如果运行队列为空且就绪队列非空，立即调度新的进程
        if (run_queue_.empty() && !ready_queue_.empty()) {
            ready_queue_.front().current_state = "Run";
            run_queue_.push(ready_queue_.front());
            ready_queue_.pop();
        }
    }

    // 完成当前进程并将其移到完成队列
    void finishCurrentProcess() {
        // 记录完成时间
        run_queue_.front().completion_time = system_cpu_time_;
        // 更新状态
        run_queue_.front().current_state = "Finish";
        // 将进程移到完成队列
        finish_queue_.push(run_queue_.front());
        // 移出运行队列
        run_queue_.pop();
    }

    // 显示当前状态
    void displayAll() const {
        displayQueue("就绪队列", ready_queue_);
        displayQueue("运行队列", run_queue_);
        displayQueue("阻塞队列", block_queue_);
        displayQueue("挂起队列", hung_queue_);
        displayQueue("完成队列", finish_queue_);
        cout << " - 当前系统CPU时间：" << system_cpu_time_ << endl;
    }

    bool isFinished() const {
        return scheduling_finished_;
    }

private:
    // 处理当就绪队列为空，而阻塞或挂起队列非空的情况
    void handleEmptyQueues() {
        char choice = 'p';  // 默认选项
        cout << endl;
        cout << "***就绪队列为空，是否结束程序？(y/n)***" << endl;
        cin >> choice;
        // 如果用户选择结束调度，设置调度完成标志
        if (choice == 'y') {
            scheduling_finished_ = true;
        }
    }

    // 解除阻塞进程
    void unblockProcess(const int id) {
        queue<PCB> tempQueue;  // 用于暂存其他阻塞进程
        bool found = false;  // 标记是否找到指定ID的进程

        // 遍历阻塞队列，寻找目标进程
        while (!block_queue_.empty()) {
            PCB p = block_queue_.front();
            block_queue_.pop();

            if (p.process_id == id) {
                // 找到目标进程，清空阻塞原因，并将其移到就绪队列
                p.block_reason = "";
                p.current_state = "Ready";
                ready_queue_.push(p);
                found = true;
            } else {
                // 如果不是目标进程，将其暂存到临时队列
                tempQueue.push(p);
            }
        }

        // 恢复阻塞队列为剩余未解除的进程
        block_queue_ = tempQueue;

        // 如果未找到目标进程，输出提示信息
        if (!found) {
            cout << "未找到阻塞进程ID：" << id << endl;
        }
    }

    // 解除挂起进程
    void unHungProcess(const int id) {
        queue<PCB> tempQueue;  // 用于暂存其他挂起进程
        bool found = false;  // 标记是否找到指定ID的进程

        // 遍历挂起队列，寻找目标进程
        while (!hung_queue_.empty()) {
            PCB p = hung_queue_.front();
            hung_queue_.pop();

            if (p.process_id == id) {
                // 找到目标进程，清空挂起原因，并将其移到就绪队列
                p.block_reason = " ";
                p.current_state = "Ready";
                ready_queue_.push(p);
                found = true;
            } else {
                // 如果不是目标进程，将其暂存到临时队列
                tempQueue.push(p);
            }
        }

        // 恢复挂起队列为剩余未解除的进程
        hung_queue_ = tempQueue;

        // 如果未找到目标进程，输出提示信息
        if (!found) {
            cout << "未找到挂起进程ID：" << id << endl;
        }
    }

    // 显示进程
    static void displayQueue(const string& q_name, queue<PCB> q)
    {
        if (!q.empty()) {
            cout << "------------" << q_name << "------------" << endl;
        }
        while (!q.empty())
        {
            PCB p = q.front();
            q.pop();
            cout << "名称:" << p.process_name << "\tID:" << p.process_id << "\t优先级:" << p.priority_level << "\t状态:" << p.current_state << endl;
            cout << "CPU使用时间:" << p.cpu_time_used << "\t总时间:" << p.total_service_time << "\t剩余时间:" << p.remaining_time << endl;
            cout << endl;
        }
    }
};

int main()
{
    // 创建进程管理器
    ProcessManager process_manager;

    cout << "==========进程模拟系统==========" << endl;

    // 选择进程添加方式
    int chance;
    cout << "1:自动添加进程\t2:手动添加进程" << endl;
    cin >> chance;
    switch (chance) {
        case 1:
            process_manager.createRandomProcesses();
            break;
        case 2:
            process_manager.createProcessByUser();
            break;
        default: ;
    }

    // 选择算法
    int algo;
    cout << "============算法选择============" << endl;
    cout << "1.先来先服务\t2.优先级\n3.时间片轮转" << endl;
    cin >> algo;

    // 开始模拟
    while (!process_manager.isFinished()) {
        // 选择算法
        process_manager.selectSchedulingAlgorithm(algo);
        // 输出当前系统及所有进程状态
        process_manager.displayAll();

        // 选择操作
        int operation;
        cout << "============操作选择============" << endl;
        cout << "1.添加进程\t2.开始调度" << endl;
        cout << "3.时间片到期\t4.其他阻塞" << endl;
        cout << "5.其他阻塞完成\t6.挂起" << endl;
        cout << "7.挂起完成\t0.退出" << endl;
        cin >> operation;
        switch (operation) {
        case 1:
            process_manager.createProcessByUser();
            break;
        case 2:
            process_manager.beginDispatch();
            break;
        case 3:
            process_manager.handleTimeSliceExpiration();
            break;
        case 4:
            process_manager.handleOtherBlockingEvents();
            break;
        case 5:
            process_manager.handleOtherBlockingEventsCompletion();
            break;
        case 6:
            process_manager.handleSuspendRequest();
            break;
        case 7:
            process_manager.handleSuspendCompletion();
            break;
        case 0:
            exit(0);
        default:
            cout << "请输入正确的操作序号！" << endl;
        }
    }

    return 0;
}
