#include "LiStack.hpp"
#include <iostream>
using namespace std;

void InitStack(LinkStNode *&S)
{
    S = (LinkStNode *)malloc(sizeof(LinkStNode));
    S->next = NULL;
}
