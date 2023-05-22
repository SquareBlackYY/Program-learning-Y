// ch8_15.c

#include <stdio.h>
#include <stdlib.h>

int searchInsert(int *nums, int numsSize, int target)
{
    int left = 0, right = numsSize - 1, mid;
    while (left <= right)
    {
        mid = left + (right - left) / 2;
        if (nums[mid] < target)
            left = mid + 1;
        else if (nums[mid] > target)
            right = mid - 1;
        else
            return mid;
    }
    return left;
}

int *insert(int *nums, int numsSize, int target, int *returnSize)
{
    int insertIdx = searchInsert(nums, numsSize, target);
    *returnSize = numsSize + 1;
    int *result = (int *)malloc(sizeof(int) * (*returnSize));
    int i;
    for (i = 0; i < insertIdx; i++)
        result[i] = nums[i];
    result[insertIdx] = target;
    for (i = insertIdx + 1; i < (*returnSize); i++)
        result[i] = nums[i - 1];
    return result;
}

void ch8_15()
{
    int numsSize, target;
    printf("请输入排序数组的元素个数：");
    scanf("%d", &numsSize);
    int *nums = (int *)malloc(sizeof(int) * numsSize);
    printf("请输入排序数组：");
    int i;
    for (i = 0; i < numsSize; i++)
        scanf("%d", &nums[i]);
    printf("请输入目标数：");
    scanf("%d", &target);

    int returnSize = 0;
    int *result = insert(nums, numsSize, target, &returnSize);

    printf("插入后的数组为：");
    for (i = 0; i < returnSize; i++)
        printf("%d ", result[i]);
    printf("\n");

    free(nums);
    free(result);
}