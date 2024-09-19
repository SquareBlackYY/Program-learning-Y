#include "BTree.hpp"
#include <iostream>
using namespace std;

// 凹入表示法输出二叉树
void indentedRepresent(BTNode *T, int level)
{
    if (T == NULL)
        return;

    // 输出当前节点
    char data = T->data;
    // 左边 ' ' 填充，右边 '#' 填充
    cout << string(level * 4, ' ') + data + string(30 - level * 4 - 1, '-') << endl;

    // 递归处理左子树和右子树
    indentedRepresent(T->lchild, level + 1);
    indentedRepresent(T->rchild, level + 1);
}

// 括号表示法创建二叉树
void CreateBTNode(BTNode *&b, char *str)
{
    BTNode *St[MaxSize], *p;
    int top = -1, k, j = 0;
    char ch;
    b = NULL;
    ch = str[j];
    while (ch != '\0')
    {
        switch (ch)
        {
        case '(':
            top++;
            St[top] = p;
            k = 1;
            break;
        case ')':
            top--;
            break;
        case ',':
            k = 2;
            break;
        default:
            p = new BTNode;
            p->data = ch;
            p->lchild = p->rchild = NULL;
            if (b == NULL)
                b = p;
            else
            {
                switch (k)
                {
                case 1:
                    St[top]->lchild = p;
                    break;
                case 2:
                    St[top]->rchild = p;
                    break;
                }
            }
        }
        j++;
        ch = str[j];
    }
}