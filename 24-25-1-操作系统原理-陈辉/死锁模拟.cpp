#include <iostream>
#include <cstdio>
#include <cstdlib>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

int processCount;                               // 进程数量
int resourceCount;                              // 资源种类数量
int max[MAX_PROCESSES][MAX_RESOURCES];          // 最大资源矩阵
int allocation[MAX_PROCESSES][MAX_RESOURCES];   // 已分配资源矩阵
int need[MAX_PROCESSES][MAX_RESOURCES];         // 需求矩阵
int available[MAX_RESOURCES];                   // 可用资源向量
bool finishProcess[MAX_PROCESSES];              // 已完成进程

void initialize();
bool checkFinish();
bool isSafe();
void requestResources();
void printCurrentState();

int main() {
    initialize();
    
    while (true)
        if (isSafe())
            requestResources();
        else {
            std::cout << "系统不在安全状态，程序结束。" << std::endl;
            break;
        } 

    return 0;
}

// 初始化资源数组
void initialize() {
    std::cout << "请输入进程数量: ";
    std::cin >> processCount;

    std::cout << "请输入资源种类数量: ";
    std::cin >> resourceCount;

    std::cout << "请输入可用资源向量: ";
    for (int i = 0; i < resourceCount; i++)
        std::cin >> available[i];

    std::cout << "请输入最大资源矩阵:" << std::endl;
    for (int i = 0; i < processCount; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < resourceCount; j++)
            std::cin >> max[i][j];
    }

    std::cout << "请输入已分配资源矩阵:" << std::endl;
    for (int i = 0; i < processCount; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < resourceCount; j++) {
            std::cin >> allocation[i][j];
            need[i][j] = max[i][j] - allocation[i][j];
            if (allocation[i][j] > max[i][j])
                std::cerr << "已分配资源不能超过最大资源。" << std::endl;
        }
    }

    printCurrentState();
}

// 检查是否所有进程都已完成
bool checkFinish() {
    for (int i = 0; i < processCount; i++)
        if (!finishProcess[i])
            return false;
    return true;
}

// 安全性检测算法
bool isSafe() {
    bool finish[MAX_PROCESSES] = {false};
    int work[MAX_RESOURCES];
    for (int i = 0; i < resourceCount; i++)
        work[i] = available[i];

    int completedProcesses = 0;
    while (completedProcesses < processCount) {
        bool found = false;
        for (int i = 0; i < processCount; i++) {
            if (!finish[i]) {
                int j;
                for (j = 0; j < resourceCount; j++)
                    if (need[i][j] > work[j])
                        break;

                if (j == resourceCount) {
                    for (int k = 0; k < resourceCount; k++)
                        work[k] += allocation[i][k];
                    finish[i] = true;
                    found = true;
                    completedProcesses++;
                    printf("P%d -> ", i);
                }
            }
        }
        if (!found) {
            std::cout << "\n找不到安全序列。" << std::endl;
            return false;
        }
    }
    std::cout << "\n系统处于安全状态。" << std::endl;
    return true;
}

// 资源分配请求
void requestResources() {
    if (checkFinish()) {
        std::cout << "所有进程资源分配完毕，程序退出。" << std::endl;
        exit(0);
    }

    int process;
    std::cout << "请输入请求资源的进程编号 (或输入-1退出): ";
    std::cin >> process;

    if (process == -1) {
        std::cout << "程序结束。" << std::endl;
        exit(0);
    }
    if (process < 0 || process >= processCount) {
        std::cout << "无效的进程编号。" << std::endl;
        return;
    }
    if (finishProcess[process]) {
        std::cout << "该进程已分配完成，无需分配资源。" << std::endl;
        return;
    }

    int request[MAX_RESOURCES];
    std::cout << "请输入资源请求向量: ";
    for (int i = 0; i < resourceCount; i++)
        std::cin >> request[i];

    for (int i = 0; i < resourceCount; i++)
        if (request[i] > need[process][i] || request[i] > available[i]) {
            std::cout << "资源请求不合理或超过当前可用资源，无法分配。" << std::endl;
            return;
        }

    for (int i = 0; i < resourceCount; i++) {
        available[i] -= request[i];
        allocation[process][i] += request[i];
        need[process][i] -= request[i];
    }

    printCurrentState();
    if (isSafe()) {
        std::cout << "资源已成功分配。" << std::endl;

        bool processComplete = true;
        for (int i = 0; i < resourceCount; i++)
            if (need[process][i] != 0) {
                processComplete = false;
                break;
            }

        if (processComplete) {
            finishProcess[process] = true;
            printf("进程 P%d 完成并释放资源。\n", process);
            for (int i = 0; i < resourceCount; i++) {
                available[i] += allocation[process][i];
                allocation[process][i] = 0;
            }
        }
    } else {
        for (int i = 0; i < resourceCount; i++) {
            available[i] += request[i];
            allocation[process][i] -= request[i];
            need[process][i] += request[i];
        }
        std::cout << "无法分配资源。" << std::endl;
    }

    printCurrentState();
}

// 打印系统状态
void printCurrentState() {
    std::cout << "当前系统状态:" << std::endl;
    printf("%-8s%-10s%-15s%-15s%-10s\n", "进程", "最大资源", "已分配资源", "需求资源", "已完成");

    for (int i = 0; i < processCount; i++) {
        printf("P%-6d", i);
        for (int j = 0; j < resourceCount; j++)
            printf("%d ", max[i][j]);
        
        printf("%-5s", "");
        for (int j = 0; j < resourceCount; j++)
            printf("%d ", allocation[i][j]);
        
        printf("%-5s", "");
        for (int j = 0; j < resourceCount; j++)
            printf("%d ", need[i][j]);
        
        printf("%-5s", "");
        printf("%s\n", finishProcess[i] ? "是" : "否");
    }

    std::cout << "系统可用资源: ";
    for (int i = 0; i < resourceCount; i++)
        std::cout << available[i] << " ";
    std::cout << std::endl;
}