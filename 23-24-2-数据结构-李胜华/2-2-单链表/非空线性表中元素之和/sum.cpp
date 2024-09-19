#include "LinkList.hpp"
#include <iostream>

ElemType sum_f(const LinkNode *);

int main()
{
    LinkNode *L;
    ElemType a[] = {2, 5, 8, 3, 6};
    CreateListR(L, a, 5);

    int sum = sum_f(L);
    std::cout << "线性表中元素的和为:" << sum << std::endl;
    
    return 0;
}

ElemType sum_f(const LinkNode *next)
{
    if (next->next == NULL)
        return next->data;
    else
        return next->data + sum_f(next->next);
}