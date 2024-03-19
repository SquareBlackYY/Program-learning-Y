#include <iostream>
using namespace std;

#include "SqList.hpp"

int main()
{
    SqList *L;
    ElemType a[] = {'a', 'b', 'c', 'd', 'e'};

    // (1) 初始化顺序表L
    InitList(L);

    // (2) 依次插入a、b、c、d、e元素
    for (int i = 0; i < 5; i++)
        ListInsert(L, i + 1, a[i]);

    // (3) 输出顺序表L
    cout << "顺序表L: ";
    DispList(L);

    // (4) 输出顺序表L的长度
    cout << "顺序表L的长度: " << ListLength(L) << endl;

    // (5) 判断顺序表L是否为空
    if (ListEmpty(L))
        cout << "顺序表L为空" << endl;
    else
        cout << "顺序表L不为空" << endl;

    // (6) 输出顺序表L的第3个元素
    ElemType e;
    GetElem(L, 3, e);
    cout << "顺序表L的第3个元素为: " << e << endl;

    // (7) 输出元素a的位置
    cout << "元素a的位置为: " << LocateElem(L, 'a') << endl;

    // (8) 在第4个元素的位置上插入f元素
    ListInsert(L, 4, 'f');

    // (9) 输出顺序表L
    cout << "插入元素后的顺序表L: ";
    DispList(L);

    // (10) 删除顺序表L的第3个元素
    ElemType deletedElem;
    ListDelete(L, 3, deletedElem);

    // (11) 输出顺序表L
    cout << "删除第3个元素后的顺序表L: ";
    DispList(L);

    // (12) 释放顺序表L
    DestroyList(L);

    return 0;
}