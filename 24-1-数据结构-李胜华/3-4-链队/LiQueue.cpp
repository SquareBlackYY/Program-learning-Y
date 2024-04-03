#include "LiQueue.hpp"
#include <iostream>
using namespace std;

void InitQueue(LinkQuNode *&q)
{
    q = (LinkQuNode *)malloc(sizeof(LinkQuNode));
    q->front = q->rear = NULL;
}

void EnQueue(LinkQuNode *& q, ElemType e)
{
    DataNode *p;
    p = (DataNode *)malloc(sizeof(DataNode));
}
