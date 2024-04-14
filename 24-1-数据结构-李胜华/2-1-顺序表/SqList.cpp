#include "SqList.hpp"
#include <iostream>

void CreateList(SqList *&L, ElemType a[], int n)
{
    int i = 0, k = 0;
    L = (SqList *)malloc(sizeof(SqList));
    while (i < n)
    {
        L->data[k] = a[i];
        k++;
        i++;
    }
    L->length = k;
}

void InitList(SqList *&L)
{
    L = (SqList *)malloc(sizeof(SqList));
    L->length = 0;
}

void DestroyList(SqList *&L)
{
    free(L);
}

bool ListEmpty(SqList *L)
{
    return (L->length == 0);
}

int ListLength(SqList *L)
{
    return L->length;
}

void DispList(SqList *L)
{
    for (int i = 0; i < L->length; i++)
        std::cout << L->data[i] << ' ';
    std::cout << std::endl;
}

bool GetElem(SqList *L, int i, ElemType &e)
{
    if (i < 1 || i > L->length)
        return false;
    e = L->data[i - 1];
    return true;
}

int LocateElem(SqList *L, ElemType e)
{
    int i = 0;
    while (i < L->length && L->data[i] != e)
        i++;
    if (i >= L->length)
        return 0;
    return i + 1;
}

bool ListInsert(SqList *&L, int i, ElemType e)
{
    int j;
    if (i < 1 || i > L->length + 1 || L->length == MaxSize)
        return false;
    i--;
    for (j = L->length; j > i; j--)
        L->data[j] = L->data[j - 1];
    L->data[i] = e;
    L->length++;
    return true;
}

bool ListDelete(SqList *&L, int i, ElemType &e)
{
    int j;
    if (i < 1 || i > L->length)
        return false;
    i--;
    e = L->data[i];
    for (j = i; j < L->length - 1; j++)
        L->data[j] = L->data[j + 1];
    L->length--;
    return true;
}

ElemType Sum_Recursion(SqList *L, int n)
{
    if (n == 1)
        return L->data[0];
    else
        return L->data[n - 1] + Sum_Recursion(L, n - 1);
}

void MergeSort_Recursion(SqList *&L, int left, int right)
{
    if (left >= right)
        return; // 递归出口
    int mid = (left + right) / 2;
    MergeSort_Recursion(L, left, mid);
    MergeSort_Recursion(L, mid + 1, right);
    Merge_Recursion(L, left, mid, right); // 合并有序表
}

void Merge_Recursion(SqList *&L, int low, int mid, int high)
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