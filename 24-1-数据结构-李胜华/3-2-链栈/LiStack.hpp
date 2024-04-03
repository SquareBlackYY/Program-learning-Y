#ifndef LISTACK_HPP
#define LISTACK_HPP

typedef char ElemType;
typedef struct linknode
{
    ElemType data;
    struct linknode *next;
} LinkStNode;

void InitStack(LinkStNode *&S);
void Push(LinkStNode *&S);

#endif