#ifndef LINKLIST_H
#define LINKLIST_H

const int MaxSize = 100;

typedef char ElemType;
typedef struct
{
    ElemType data[MaxSize];
    int length;
} LinkList;

void InitList(LinkList *&L);
void DestroyList(LinkList *&L);
bool ListEmpty(LinkList *L);
int ListLength(LinkList *L);
void DispList(LinkList *L);
bool GetElem(LinkList *L, int i, ElemType &e);
int LocateElem(LinkList *L, ElemType e);
bool ListInsert(LinkList *&L, int i, ElemType e);
bool ListDelete(LinkList *&L, int i, ElemType &e);

#endif