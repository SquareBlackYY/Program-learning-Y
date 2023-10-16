#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    const long num_chars = 75 * 1024 * 1024;

    srand(time(NULL));

    FILE *file = fopen("input.txt", "w");

    for (long i = 0; i < num_chars; i++) {
        int random_num = rand() % 16;
        char hex_char = (random_num < 10) ? ('0' + random_num) : ('a' + random_num - 10);
        fprintf(file, "%c", hex_char);
    }

    fclose(file);

    printf("已生成 %ld 个十六进制字符并写入 input.txt\n", num_chars);

    return 0;
}
