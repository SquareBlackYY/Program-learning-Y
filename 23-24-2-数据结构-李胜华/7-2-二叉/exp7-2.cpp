#include "BTree.hpp"
#include <iostream>
using namespace std;

int main()
{
    BTNode *b;
    char str[MaxSize] = "A(B,C(,D))";

    CreateBTNode(b, str);

    // 输出二叉树的凹入表示法
    indentedRepresent(b, 0);

    return 0;
}