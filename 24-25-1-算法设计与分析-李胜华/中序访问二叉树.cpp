#include <iostream>
#include <cstddef>
using namespace std;

typedef char ElemType;

// 二叉树节点结构
struct BTNode
{
    ElemType data;
    BTNode *lchild;
    BTNode *rchild;
    BTNode() : data(0), lchild(NULL), rchild(NULL) {}
    BTNode(int x) : data(x), lchild(NULL), rchild(NULL) {}
};

int main()
{

    return 0;
}


//中序遍历
void inorderTraversal(BTNode* node)
{
    if (!node)
        return;

    inorderTraversal(node->lchild);
    cout << node->data << " ";
    inorderTraversal(node->rchild);
}