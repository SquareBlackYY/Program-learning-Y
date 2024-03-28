#include "SqList.hpp"
#include <iostream>
using namespace std;

// 初始化线性表，构造一个空的线性表L。
void InitList(SqList *&L)
{
    L = (SqList *)malloc(sizeof(SqList));
    L->length = 0;
}

// 销毁线性表，释放为线性表L分配的内存空间。
void DestroyList(SqList *&L)
{
    delete L;
}

// 判断线性表是否为空表，若L为空表，则返回真，否则返回假。
bool ListEmpty(SqList *L)
{
    return (L->length == 0);
}

// 求线性表的长度，返回L中元素的个数。
int ListLength(SqList *L)
{
    return (L->length);
}

// 输出线性表，当线性表L不为空时顺序输出L中各元素值。
void DispList(SqList *L)
{
    for (int i = 0; i < L->length; i++)
        cout << L->data[i] << ' ';
    cout << endl;
}

// 按序号求线性表中元素，用e返回L中第i(1≤i≤n)个元素值。
bool GetElem(SqList *L, int i, ElemType &e)
{
    if (i < 1 || i > L->length)
        return false;
    e = L->data[i - 1];
    return true;
}

// 按元素值查找，返回L中第一个值为e相等的元素序号。
int LocateElem(SqList *L, ElemType e)
{
    int i = 0;
    while (i < L->length && L->data[i] != e)
        i++;
    if (i >= L->length)
        return 0;
    return i + 1;
}

// 插入元素，在L的第i(1≤i≤n+1)个位置插入一个新元素e。
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

// 删除元素，删除L的第i(1≤i≤n)个元素，并用e返回该元素值。
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