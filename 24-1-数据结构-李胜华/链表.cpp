#include <iostream>
using namespace std;

typedef int ElemType;
typedef struct LNode
{
    ElemType data;
    struct LNode *next;
} LNode, *LinkList;

int main()
{
    LinkList L, p;
    L = new LNode;
    for (int i = 1; i <= 5; i++)
    {
        p = new LNode;
        cin >> p->data;
        p->next = L->next;
        L->next = p;
    }
    p = L->next;
    while (p)
    {
        cout<<p->data<<endl;
        p = p->next;
    }
    return 0;
}