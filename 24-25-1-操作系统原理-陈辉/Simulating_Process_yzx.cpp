#include <iostream>
#include <queue>
#include <algorithm>
#include <cstring>
using namespace std;

const int MAXN = 1000;  // 最大容纳的进程数量

class ProcessScheduler {
public:
    typedef struct node {
        char name[20];          // 进程名
        int id = 0;             // 进程编号
        int priority = 0;       // 进程优先级
        int ArriTime = 0;       // 进程到达时间
        int serviceTime = 0;    // 进程所需总时间
        int needTime = 0;       // 进程剩余时间
        int CPUTime = 0;        // 进程 CPU 使用时间
        char state[20];         // 进程状态
        int finishTime = 0;     // 进程完成时的系统 CPU 时间
        char blockCause[30];    // 阻塞原因
    } PCB;

private:
    queue<PCB> processQueue, readyQueue, blockQueue, runQueue, finishQueue, hungQueue;
    int ID = 1;             // 总进程 ID
    int time = 0;           // 系统 CPU 时间
    int algorithm = 0;      // 调度算法
    int timeslice = 0;      // 时间片
    bool finish = false;    // 是否结束进程调度

    struct cmp1 {
        bool operator()(const PCB &p1, const PCB &p2) {
            return p1.ArriTime < p2.ArriTime;
        }
    };

    struct cmp2 {
        bool operator()(const PCB &p1, const PCB &p2) {
            return p1.priority > p2.priority;
        }
    };

public:
    // 初始化进程
    void Init() {
        PCB p1, p2, p3, p4, p5;
        strcpy(p1.name, "a");
        p1.id = ID++;
        p1.priority = 8;
        p1.ArriTime = 2;
        p1.needTime = 4;
        p1.serviceTime = 4;
        processQueue.push(p1);

        strcpy(p2.name, "b");
        p2.id = ID++;
        p2.priority = 4;
        p2.ArriTime = 6;
        p2.needTime = 3;
        p2.serviceTime = 3;
        processQueue.push(p2);

        strcpy(p3.name, "c");
        p3.id = ID++;
        p3.priority = 7;
        p3.ArriTime = 3;
        p3.needTime = 6;
        p3.serviceTime = 6;
        processQueue.push(p3);

        strcpy(p4.name, "d");
        p4.id = ID++;
        p4.priority = 1;
        p4.ArriTime = 0;
        p4.needTime = 1;
        p4.serviceTime = 1;
        processQueue.push(p4);

        strcpy(p5.name, "e");
        p5.id = ID++;
        p5.priority = 2;
        p5.ArriTime = 2;
        p5.needTime = 2;
        p5.serviceTime = 2;
        processQueue.push(p5);
    }

    // 添加进程
    void addProcess() {
        int n;
        cout << "请输入要添加的进程数：" << endl;
        cin >> n;
        for (int i = 0; i < n; i++) {
            PCB p;
            cout << "请输入第" << i + 1 << "个进程名、进程优先级、进程到达时间、进程需要运行时间:" << endl;
            cin >> p.name >> p.priority >> p.ArriTime >> p.serviceTime;
            p.ArriTime += time;     // 新添加进程的到达时间要加上当前的系统 CPU 时间
            p.id = ID++;
            p.needTime = p.serviceTime;
            processQueue.push(p);
        }
    }

    // 系统开始调度
    void beginDispatch() {
        if (runQueue.empty()) {
            if (!readyQueue.empty()) {
                strcpy(readyQueue.front().state, "Run");
                runQueue.push(readyQueue.front());
                readyQueue.pop();
            } else if (!blockQueue.empty() || !hungQueue.empty()) {
                handleEmptyQueues();
            } else {
                finish = true;
                return;
            }
        } else if (!runQueue.empty()) {
            processRunQueue();
        }
    }

    // 时间片到期
    void timesliceDaoqi() {
        if (!runQueue.empty()) {
            if (runQueue.front().needTime != 0) {
                PCB p = runQueue.front();
                runQueue.pop();
                strcpy(p.state, "Ready");
                if (algorithm == 1) {   // 先来先服务时，时间片到期则放到就绪队列队尾
                    p.ArriTime = readyQueue.back().ArriTime + 1;
                }
                readyQueue.push(p);
            }
        } else {
            cout << "当前无运行进程" << endl;
        }
    }

    // 其他阻塞事件
    void ElseZC() {
        if (!runQueue.empty()) {
            if (runQueue.front().needTime != 0) {
                strcpy(runQueue.front().blockCause, "其他阻塞事件");
                strcpy(runQueue.front().state, "Wait");
                blockQueue.push(runQueue.front());
                runQueue.pop();
            }
        } else {
            cout << "当前无运行进程" << endl;
        }
    }

    // 其他阻塞事件完成
    void ElseZCFinish() {
        if (blockQueue.empty()) {
            cout << "目前无阻塞进程" << endl;
        } else {
            int id;
            cout << "请输入其他阻塞事件完成的进程ID" << endl;
            cin >> id;
            unblockProcess(id);
        }
    }

    // 挂起
    void Hung() {
        if (!runQueue.empty()) {
            if (runQueue.front().needTime != 0) {
                strcpy(runQueue.front().blockCause, "请求挂起");
                strcpy(runQueue.front().state, "Hung");
                hungQueue.push(runQueue.front());
                runQueue.pop();
            }
        } else {
            cout << "当前无运行进程" << endl;
        }
    }

    // 挂起完成
    void HungFinish() {
        if (hungQueue.empty()) {
            cout << "目前无挂起进程" << endl;
        } else {
            int id;
            cout << "请输入挂起完成的进程ID" << endl;
            cin >> id;
            unHungProcess(id);
        }
    }

    // 选择调度算法
    void selectAlgorithm(int algo) {
        algorithm = algo;
        if (algorithm == 1) 
            FCFS();
        else if (algorithm == 2)
            SJF();
        else if (algorithm == 3) {
            if (timeslice == 0) {
                cout << "请输入时间片大小：" << endl;
                cin >> timeslice;
            }
            RR();
        }
    }

    // 先来先服务
    void FCFS() {
        // 根据到达时间排序就绪队列
        PCB temp[MAXN];
        int n = 0;
        while (!processQueue.empty()) {
            temp[n++] = processQueue.front();
            processQueue.pop();
        }
        while (!readyQueue.empty()) {
            temp[n++] = readyQueue.front();
            readyQueue.pop();
        }
        sort(temp, temp + n, cmp1());
        for (int i = 0; i < n; i++) {
            strcpy(temp[i].state, "Ready");
            readyQueue.push(temp[i]);
        }
    }

    // 优先级调度
    void SJF() {
        // 根据优先级排序就绪队列
        PCB temp[MAXN];
        int n = 0;
        while (!processQueue.empty()) {
            temp[n++] = processQueue.front();
            processQueue.pop();
        }
        while (!readyQueue.empty()) {
            temp[n++] = readyQueue.front();
            readyQueue.pop();
        }
        sort(temp, temp + n, cmp2());
        for (int i = 0; i < n; i++) {
            strcpy(temp[i].state, "Ready");
            readyQueue.push(temp[i]);
        }
    }

    // 时间片轮转
    void RR() {
        // 将进程队列移动到就绪队列，且不改变顺序
        while (!processQueue.empty()) {
            PCB p = processQueue.front();
            processQueue.pop();
            strcpy(p.state, "Ready");
            readyQueue.push(p);
        }
    }

    // 处理运行中的进程
    void processRunQueue() {
        time++;
        runQueue.front().CPUTime++;
        runQueue.front().needTime--;
        
        // 处理不同调度算法
        if (runQueue.front().needTime == 0) {
            finishCurrentProcess();
        }
        else {
            if (algorithm == 1 || algorithm == 2) {
                return;
            } else if (algorithm == 3) {
                if (runQueue.front().CPUTime != 0 && runQueue.front().CPUTime % timeslice == 0) {
                    strcpy(runQueue.front().state, "Ready");
                    readyQueue.push(runQueue.front());
                    runQueue.pop();
                }
            }
        }
        
        // 运行队列为空，就绪队列非空，立刻压入运行队列
        if (runQueue.empty() && !readyQueue.empty()) {
            strcpy(readyQueue.front().state, "Run");
            runQueue.push(readyQueue.front());
            readyQueue.pop();
        }
    }

    // 完成当前运行进程
    void finishCurrentProcess() {
        runQueue.front().finishTime = time;
        strcpy(runQueue.front().state, "Finish");
        finishQueue.push(runQueue.front());
        runQueue.pop();
    }

    // 显示当前状态
    void displayAll() {
        displayQueue("就绪队列", readyQueue);
        displayQueue("运行队列", runQueue);
        displayQueue("阻塞队列", blockQueue);
        displayQueue("挂起队列", hungQueue);
        displayQueue("完成队列", finishQueue);
        cout << "当前系统CPU时间：" << time << endl;
    }

    bool isFinished() {
        return finish;
    }

private:
    // 就绪队列为空，阻塞或挂起队列非空的处理
    void handleEmptyQueues() {
        char chance = 'p';
        cout << endl;
        cout << "********就绪队列为空，是否结束程序？(y/n)********" << endl;
        cin >> chance;
        if (chance == 'y') {
            finish = true;
        }
    }

    // 阻塞完成
    void unblockProcess(int id) {
        queue<PCB> tempQueue;
        bool found = false;
        while (!blockQueue.empty()) {
            PCB p = blockQueue.front();
            blockQueue.pop();
            if (p.id == id) {
                strcpy(p.blockCause, " ");
                strcpy(p.state, "Ready");
                readyQueue.push(p);
                found = true;
            } else {
                tempQueue.push(p);
            }
        }
        blockQueue = tempQueue;
        if (!found) cout << "未找到阻塞进程ID：" << id << endl;
    }

    // 挂起完成
    void unHungProcess(int id) {
        queue<PCB> tempQueue;
        bool found = false;
        while (!hungQueue.empty()) {
            PCB p = hungQueue.front();
            hungQueue.pop();
            if (p.id == id) {
                strcpy(p.blockCause, " ");
                strcpy(p.state, "Ready");
                readyQueue.push(p);
                found = true;
            } else {
                tempQueue.push(p);
            }
        }
        hungQueue = tempQueue;
        if (!found) cout << "未找到阻塞进程ID：" << id << endl;
    }

    // 显示进程
    void displayQueue(const char* queueName, queue<PCB> q) {
        cout << "----------------" << queueName << "----------------" << endl;
        while (!q.empty()) {
            PCB p = q.front();
            q.pop();
            cout << p.name << "  ID: " << p.id << "  优先级: " << p.priority << "  状态: " << p.state << "  CPU使用时间: " << p.CPUTime << "  总时间: " << p.serviceTime << "  剩余时间: " << p.needTime << endl;
        }
    }
};

int main() {
    ProcessScheduler scheduler;

    cout << "********进程状态转换系统********" << endl;
    int chance;
    cout << "1:自动添加进程    2:手动添加进程" << endl;
    cin >> chance;
    switch (chance) {
    case 1:
        scheduler.Init();
        break;
    case 2:
        scheduler.addProcess();
        break;
    }
    
    int algo;
    cout << "选择算法：1.先来先服务 2.优先级 3.时间片轮转" << endl;
    cin >> algo;

    while (!scheduler.isFinished()) {
        scheduler.selectAlgorithm(algo);

        scheduler.displayAll();

        int operation;
        cout << "选择操作：1.添加进程 2.开始调度 3.时间片到期 4.其他阻塞 5.其他阻塞完成 6.挂起 7.挂起完成 0.退出" << endl;
        cin >> operation;
        switch (operation) {
        case 1:
            scheduler.addProcess();
            break;
        case 2:
            scheduler.beginDispatch();
            break;
        case 3:
            scheduler.timesliceDaoqi();
            break;
        case 4:
            scheduler.ElseZC();
            break;
        case 5:
            scheduler.ElseZCFinish();
            break;
        case 6:
            scheduler.Hung();
            break;
        case 7:
            scheduler.HungFinish();
            break;
        case 0:
            exit(0);
        default:
            cout << "请输入正确的操作序号！" << endl;
        }
    }

    return 0;
}
