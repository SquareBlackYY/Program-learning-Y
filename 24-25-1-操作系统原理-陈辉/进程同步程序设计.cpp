#include <pthread.h>
#include <semaphore.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 5       // 缓冲区大小

int buffer[BUFFER_SIZE];    // 缓冲区
int count = 0;              // 当前缓冲区中产品数量

// 信号量
sem_t *mutex;                // 互斥信号量, 实现对缓冲区的互斥访问
sem_t *empty;                // 空位信号量, 表示空闲缓冲区的数量
sem_t *full;                 // 满位信号量, 表示非空缓冲区的数量

void *producer(void *arg) {
    int id = *(int *)arg;
    unsigned int seed = time(NULL) + id;


    while (1) {
        int item = rand_r(&seed) % 100; // 生成随机产品

        sem_wait(empty);                // 等待空位
        sem_wait(mutex);              // 进入临界区

        sleep(1);                       // 模拟生产时间
        buffer[count++] = item;         // 将产品放入缓冲区
        std::cout << "生产者[" << id << "] 放入:[" << item << "]" << std::endl;

        sem_post(mutex);                // 离开临界区
        sem_post(full);                 // 增加满位
    }

    return NULL;
}

void *consumer(void *arg) {
    int id = *(int *)arg;

    while (1) {
        sem_wait(full);                 // 等待满位
        sem_wait(mutex);                // 进入临界区

        int item = buffer[--count];     // 从缓冲区取出产品
        sleep(1);                       // 模拟消费时间
        std::cout << "消费者[" << id << "] 取出:[" << item << "]" << std::endl;

        sem_post(mutex);                // 离开临界区
        sem_post(empty);                // 增加空位
    }

    return NULL;
}

int main() {
    int producer_num = 5, consumer_num = 5;
    pthread_t producer_id[producer_num], consumer_id[consumer_num];   // 存储线程的标识符
    int producer_name[producer_num], consumer_name[consumer_num];     // 存储生产者和消费者的名称
    for (int i = 0; i < producer_num; i++)
        producer_name[i] = i + 1;
    for (int i = 0; i < consumer_num; i++)
        consumer_name[i] = i + 1;

    // 清除缓存
    sem_unlink("/empty");
    sem_unlink("/full");
    sem_unlink("/mutex");
    
    // 初始化信号量
    empty = sem_open("/empty", O_CREAT, 0644, BUFFER_SIZE);
    full = sem_open("/full", O_CREAT, 0644, 0);
    mutex = sem_open("/mutex", O_CREAT, 0644, 1);

    // 创建线程
    for (int i = 0; i < producer_num; i++)
        pthread_create(&producer_id[i], NULL, producer, &producer_name[i]);
    for (int i = 0; i < consumer_num; i++)
        pthread_create(&consumer_id[i], NULL, consumer, &consumer_name[i]);
    
    // 等待线程完成
    for (int i = 0; i < producer_num; i++)
        pthread_join(producer_id[i], NULL);
    for (int i = 0; i < consumer_num; i++)
        pthread_join(consumer_id[i], NULL);

    // 销毁信号量
    sem_close(empty);
    sem_close(full);
    sem_close(mutex);
    sem_unlink("/empty");
    sem_unlink("/full");
    sem_unlink("/mutex");

    return 0;
}