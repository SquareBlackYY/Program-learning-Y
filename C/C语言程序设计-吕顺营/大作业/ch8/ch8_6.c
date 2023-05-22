// ch8_6.c

#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

void swap_max_and_min(int *arr, int n)
{
    int max_index = 0, min_index = 0, i;
    for (i = 1; i < n; i++)
    {
        if (arr[i] > arr[max_index])
            max_index = i;
        if (arr[i] < arr[min_index])
            min_index = i;
    }
    swap(&arr[max_index], &arr[min_index]);
}

void ch8_6()
{
    int *arr = (int *)malloc(sizeof(int) * 10), i;

    printf("请输入10个整数：\n");

    for (i = 0; i < 10; i++)
        scanf("%d", &arr[i]);

    swap_max_and_min(arr, 10);

    printf("互换后的数组为：\n");

    for (i = 0; i < 10; i++)
        printf("%d ", arr[i]);

    printf("\n");

    free(arr);
}