#ifndef SQQUEUE_HPP
#define SQQUEUE_HPP

const int MaxSize = 100;

typedef char ElemType;
typedef struct
{
    ElemType data[MaxSize];
    int front, rear;
} SqQueue;

#endif