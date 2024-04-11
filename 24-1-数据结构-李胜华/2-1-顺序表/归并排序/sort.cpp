#include "SqList.hpp"
#include <iostream>

void MergeSort(SqList *&, int, int);
void Merge(SqList *&, int, int, int);

int main()
{
    
    SqList *L;
    ElemType a[] = {2, 5, 8, 3, 6};
    CreateList(L, a, 5);
    MergeSort(L, 0, 4);
    for (int i = 0; i < L->length; i++)
        std::cout << L->data[i] << " ";
        
    return 0;
}

void MergeSort(SqList *&L, int left, int right)
{
    if (left >= right)
        return; // 递归出口
    int mid = (left + right) / 2;
    MergeSort(L, left, mid);
    MergeSort(L, mid + 1, right);
    Merge(L, left, mid, right); // 合并有序表
}

void Merge(SqList *&L, int low, int mid, int high)
{
    int i = low, j = mid + 1, k = 0;
    ElemType *t;
    t = new ElemType[high - low + 1];
    while (i <= mid && j <= high)
        if (L->data[i] <= L->data[j])
            t[k++] = L->data[i++];
        else
            t[k++] = L->data[j++];
    while (i <= mid)
        t[k++] = L->data[i++];
    while (j <= high)
        t[k++] = L->data[j++];
    for (i = low, k = 0; i <= high; i++, k++)
        L->data[i] = t[k];
    delete[] t;
}