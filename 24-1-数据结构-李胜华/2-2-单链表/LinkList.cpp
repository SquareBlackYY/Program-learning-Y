#include "LinkList.hpp"
#include <iostream>
using namespace std;

void InitList(LinkNode *&L)
{
    L = (LinkNode *)malloc(sizeof(LinkNode));
    L->next = NULL;
}

void DestroyList(LinkNode *&L)
{
    LinkNode *pre = L, *p = L->next;
    while (p != NULL)
    {
        free(pre);
        pre = p;
        p = pre->next;
    }
    free(pre);
}

bool ListEmpty(LinkNode *L)
{    
    return (L->next == NULL);
}

int ListLength(LinkNode *L)
{
    int n = 0;
    LinkNode *p = L;
    while(p->next != NULL)
    {
        n++;
        p = p->next;
    }
    return n;
}

void DispList(LinkNode *L)
{
    LinkNode *p = L->next;
    
}
