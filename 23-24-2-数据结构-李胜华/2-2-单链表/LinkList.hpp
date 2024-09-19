#ifndef LINKLIST_HPP
#define LINKLIST_HPP

const int MaxSize = 100;

typedef char ElemType;
typedef struct LNode
{
    ElemType data;
    struct LNode *next;
} LinkNode;

void InitList(LinkNode *&L);
void DestroyList(LinkNode *&L);
bool ListEmpty(LinkNode *L);
int ListLength(LinkNode *L);
void DispList(LinkNode *L);
bool GetElem(LinkNode *L, int i, ElemType &e);
int LocateElem(LinkNode *L, ElemType e);
bool ListInsert(LinkNode *&L, int i, ElemType e);
bool ListDelete(LinkNode *&L, int i, ElemType &e);

#endif