#include "SqList.hpp"
#include <iostream>
using namespace std;

void InitList(SqList *&L)
{
    L = (SqList *)malloc(sizeof(SqList));
    L->length = 0;
}

void DestroyList(List *&L)
{
    delete L;
}

bool ListEmpty(List L)
{
    return (L.length == 0);
}

int ListLength(List L)
{
    int sum = 0;
    for (int i : L.a)
        sum++;
    return sum;
}

bool DispList(List L)
{
    if (L.length==0)
		return false;
    for (int i : L.a)
        cout << i << ' ';
    return ture;
}

bool GetElem(List L, int i, int &e)
{
    if (i > L.length)
        return false;
    e = L.a[i - 1];
    return ture;
}

int LocateElem(List L, int e)
{
    int i;
    for (i = 0; i < L.length; i++)
        if (L.a[i] == e)
            return i;
    return -1;
}

void ListInsert(List &L, int i, int e)
{
    int j = L.length;
    for (; j > i - 1; j--)
        L.a[j] = L.a[j - 1];
    L.a[i - 1] = e;
    L.length++;
}

void ListDelete(List &L, int i, int &e)
{
    int j = 0;
    e = L.a[i];
    for (j = i; j < L.length; j++)
        L.a[j] = L.a[j + 1];
    L.length--;
}