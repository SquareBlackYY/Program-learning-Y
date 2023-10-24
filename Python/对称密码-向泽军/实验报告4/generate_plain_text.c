#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    clock_t start_time, end_time;
    double execution_time;
    const long num_chars = 200 * 1024 * 1024;

    srand(time(NULL));

    FILE *file = fopen("input.txt", "w");

    start_time = clock();

    for (long i = 0; i < num_chars; i++)
    {
        int random_num = rand() % 16;
        char hex_char = (random_num < 10) ? ('0' + random_num) : ('a' + random_num - 10);
        fprintf(file, "%c", hex_char);
    }

    end_time = clock();

    fclose(file);

    execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("已生成 %.2f MB数据并写入 input.txt, 用时: %.2f s\n", num_chars / 2.0 / 1024 / 1024, execution_time);

    return 0;
}
