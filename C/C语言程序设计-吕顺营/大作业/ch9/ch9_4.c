// ch9_4.c

#include <stdio.h>
#include <stdlib.h>

void max_min_swap(int *arr, int len)
{
    int max = arr[0], min = arr[0];
    int max_index = 0, min_index = 0, i;
    for (i = 1; i < len; i++)
    {
        if (arr[i] > max)
        {
            max = arr[i];
            max_index = i;
        }
        if (arr[i] < min)
        {
            min = arr[i];
            min_index = i;
        }
    }
    printf("最大值为%d，最小值为%d。\n", max, min);
    int temp = arr[max_index];
    arr[max_index] = arr[min_index];
    arr[min_index] = temp;
}

void ch9_4()
{
    int *arr = malloc(10 * sizeof(int)), i;

    printf("请输入10个整数：");
    for (i = 0; i < 10; i++)
        scanf("%d", &arr[i]);

    max_min_swap(arr, 10);

    printf("交换后的数组为：");
    for (i = 0; i < 10; i++)
        printf("%d ", arr[i]);
    printf("\n");

    free(arr);
}