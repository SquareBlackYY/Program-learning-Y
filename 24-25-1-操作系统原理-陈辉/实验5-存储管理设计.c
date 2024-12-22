#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define NUM_INSTRUCTIONS 320        // 指令数
#define INSTRUCTIONS_PER_PAGE 10    // 每页指令数

static double FIFO_hit_rate = 0, LRU_hit_rate = 0, OPT_hit_rate = 0, LFU_hit_rate = 0;

// 产生指令
void generateInstructions(int instructions[]) {
    // 起始位置
    int m = NUM_INSTRUCTIONS / 2;

    for (int i = 0; i < NUM_INSTRUCTIONS / 4; i++) {
        const int j = i * 4;
        instructions[j] = m;
        // 顺序执行
        instructions[j + 1] = m + 1;
        // 前地址, 随机生成一个值并映射到前地址中
        instructions[j + 2] = instructions[j] * (rand() / (float) RAND_MAX);
        // 顺序执行
        instructions[j + 3] = instructions[j + 2] + 1;
        // 后地址, 随机生成一个值并映射到后地址中
        m = instructions[j + 3] + (NUM_INSTRUCTIONS - 1 - instructions[j + 3]) * (rand() / (float) RAND_MAX);
    }
}

// 将指令地址转换为页地址流
void generatePageAddressStream(const int instructions[], int page_addresses[]) {
    for (int i = 0; i < NUM_INSTRUCTIONS; i++)
        // 根据页大小获取页地址
        page_addresses[i] = instructions[i] / INSTRUCTIONS_PER_PAGE;
}

// 先进先出算法 (FIFO)
void FIFO(const int page_addresses[], int memory_size) {
    int memory[memory_size];
    int page_faults = 0;

    memset(memory, -1, memory_size * sizeof(int));

    int current_index = 0;
    for (int i = 0; i < NUM_INSTRUCTIONS; i++) {
        int page = page_addresses[i];
        bool found = false;

        // 检查页面是否在内存中
        for (int j = 0; j < memory_size; j++)
            if (memory[j] == page) {
                found = true;
                break;
            }

        if (!found) {
            // 直接置换
            memory[current_index] = page;
            current_index = (current_index + 1) % memory_size;
            page_faults++;
        }
    }

    double hit_rate = 1.0 - (double) page_faults / NUM_INSTRUCTIONS;
    FIFO_hit_rate += hit_rate;
    printf("| FIFO %.2f\t", hit_rate);
}

// 最近最久未使用算法 (LRU)
void LRU(const int page_addresses[], int memory_size) {
    int memory[memory_size];
    int page_faults = 0;
    // 最后使用时间
    int last_used[memory_size];

    memset(memory, -1, memory_size * sizeof(int));
    memset(last_used, -1, memory_size * sizeof(int));

    for (int i = 0; i < NUM_INSTRUCTIONS; i++) {
        const int page = page_addresses[i];
        bool found = false;

        // 检查页面是否在内存中
        for (int j = 0; j < memory_size; j++)
            if (memory[j] == page) {
                found = true;
                // 更新最后使用的时间(模拟CPU时间)
                last_used[j] = i;
                break;
            }

        if (!found) {
            // 找到最久未使用的页面
            int min_index = 0;
            for (int j = 1; j < memory_size; j++)
                if (last_used[j] < last_used[min_index])
                    min_index = j;

            memory[min_index] = page;
            last_used[min_index] = i;
            page_faults++;
        }
    }

    double hit_rate = 1.0 - (double) page_faults / NUM_INSTRUCTIONS;
    LRU_hit_rate += hit_rate;
    printf("| LRU %.2f\t", hit_rate);
}

// 最佳淘汰算法 (OPT)
void OPT(const int page_addresses[], int memory_size) {
    int memory[memory_size];
    int page_faults = 0;

    memset(memory, -1, memory_size * sizeof(int));

    for (int i = 0; i < NUM_INSTRUCTIONS; i++) {
        const int page = page_addresses[i];
        bool found = false;

        // 检查页面是否在内存中
        for (int j = 0; j < memory_size; j++)
            if (memory[j] == page) {
                found = true;
                break;
            }

        if (!found) {
            if (page_faults < memory_size) {
                memory[page_faults] = page;
                page_faults++;
            } else {
                // 预处理下一次访问情况
                int next_access[memory_size];
                for (int j = 0; j < memory_size; j++) {
                    next_access[j] = NUM_INSTRUCTIONS; // 默认不再访问
                    for (int k = i + 1; k < NUM_INSTRUCTIONS; k++) {
                        if (page_addresses[k] == memory[j]) {
                            next_access[j] = k;
                            break;
                        }
                    }
                }

                // 找到最远的页面
                int replace_index = 0;
                int furthest = next_access[0];
                for (int j = 1; j < memory_size; j++) {
                    if (next_access[j] > furthest) {
                        furthest = next_access[j];
                        replace_index = j;
                    }
                }

                memory[replace_index] = page;
            }
        }
    }

    double hit_rate = 1.0 - (double) page_faults / NUM_INSTRUCTIONS;
    OPT_hit_rate += hit_rate;
    printf("| OPT %.2f\t", hit_rate);
}

// 最不经常使用算法 (LFU)
void LFU(const int page_addresses[], int memory_size) {
    int memory[memory_size];
    int page_faults = 0;
    // 访问频率
    int freq[memory_size];

    memset(memory, -1, memory_size * sizeof(int));
    memset(freq, 0, memory_size * sizeof(int));

    for (int i = 0; i < NUM_INSTRUCTIONS; i++) {
        const int page = page_addresses[i];
        bool found = false;

        // 检查页面是否在内存中
        for (int j = 0; j < memory_size; j++)
            if (memory[j] == page) {
                found = true;
                freq[j]++;
                break;
            }

        if (!found) {
            // 找到访问频率最少的页面
            int min_index = 0;
            for (int j = 1; j < memory_size; j++)
                if (freq[j] < freq[min_index])
                    min_index = j;

            memory[min_index] = page;
            freq[min_index] = 1;
            page_faults++;
        }
    }

    double hit_rate = 1.0 - (double) page_faults / NUM_INSTRUCTIONS;
    LFU_hit_rate += hit_rate;
    printf("| LFU %.2f\t", hit_rate);
}

int main() {
    srand(time(NULL));

    // 生成指令
    int instructions[NUM_INSTRUCTIONS];
    generateInstructions(instructions);

    // 将指令转化为页地址流
    int page_addresses[NUM_INSTRUCTIONS];
    generatePageAddressStream(instructions, page_addresses);

    // 输出不同内存容量下的命中率
    const int memory_size[] = {4, 8, 16, 32};
    for (int i = 0; i < sizeof(memory_size) / sizeof(memory_size[0]); i++) {
        printf("实存容量: % 3d 页\t命中率: ", memory_size[i]);
        FIFO(page_addresses, memory_size[i]);
        LRU(page_addresses, memory_size[i]);
        OPT(page_addresses, memory_size[i]);
        LFU(page_addresses, memory_size[i]);
        printf("\n");
    }

    printf("\t\t    平均命中率: | FIFO %.2f\t| LRU %.2f\t| OPT %.2f\t| LFU %.2f\n",
           FIFO_hit_rate / sizeof(memory_size) * sizeof(memory_size[0]),
           LRU_hit_rate / sizeof(memory_size) * sizeof(memory_size[0]),
           OPT_hit_rate / sizeof(memory_size) * sizeof(memory_size[0]),
           LFU_hit_rate / sizeof(memory_size) * sizeof(memory_size[0]));

    return 0;
}
