#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define TEST_BLOCK_SIZE (1ULL << 25)

int main() {
    FILE *file = fopen("plaintext.bin", "wb");
    if (!file) {
        perror("无法创建plaintext.bin");
        return 1;
    }

    // 分配内存并填充随机值
    uint8_t *plaintext = (uint8_t *)malloc(TEST_BLOCK_SIZE * 16 * sizeof(uint8_t));
    if (!plaintext) {
        perror("内存分配失败");
        fclose(file);
        return 1;
    }

    srand((unsigned)time(NULL));
    for (size_t i = 0; i < TEST_BLOCK_SIZE * 16; ++i) {
        plaintext[i] = rand() % 256;
    }

    fwrite(plaintext, sizeof(uint8_t), TEST_BLOCK_SIZE * 16, file);
    free(plaintext);
    fclose(file);

    printf("随机数据写入到plaintext.bin\n");
    return 0;
}