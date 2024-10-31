#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

bool compare_files(const char *file1, const char *file2) {
    FILE *f1 = fopen(file1, "rb");
    FILE *f2 = fopen(file2, "rb");

    if (!f1 || !f2) {
        perror("无法打开文件");
        return false;
    }

    // 获取file1的文件大小
    fseek(f1, 0, SEEK_END);
    size_t file_size = ftell(f1);
    fseek(f1, 0, SEEK_SET);

    uint8_t *buffer1 = (uint8_t *)malloc(file_size);
    uint8_t *buffer2 = (uint8_t *)malloc(file_size);

    if (!buffer1 || !buffer2) {
        perror("内存分配失败");
        fclose(f1);
        fclose(f2);
        return false;
    }

    fread(buffer1, sizeof(uint8_t), file_size, f1);
    fread(buffer2, sizeof(uint8_t), file_size, f2);

    bool result = (memcmp(buffer1, buffer2, file_size) == 0);

    free(buffer1);
    free(buffer2);
    fclose(f1);
    fclose(f2);
    return result;
}

int main() {
    bool result = compare_files("plaintext.bin", "plaintext_decrypted.bin");
    printf("%s\n", result ? "true" : "false");
    return 0;
}