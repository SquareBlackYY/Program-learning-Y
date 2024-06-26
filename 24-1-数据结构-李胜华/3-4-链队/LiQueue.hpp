#ifndef LIQUEUE_HPP
#define LIQUEUE_HPP

typedef char ElemType;
typedef struct qnode
{
    ElemType data;
    struct qnode *next;
} DataNode;
typedef struct
{
    DataNode *front;
    DataNode *rear;
} LinkQuNode;

void InitQueue(LinkQuNode *&q);
void EnQueue(LinkQuNode *&q, ElemType e);

#endif