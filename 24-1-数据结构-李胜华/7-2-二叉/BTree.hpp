#include <cstddef>
#ifndef BTREE_HPP
#define BTREE_HPP

#define MaxSize 50

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

void indentedRepresent(BTNode *T, int level); // 凹入表示法输出二叉树
void CreateBTNode(BTNode *&b, char *str);     // 括号表示法创建二叉树

#endif