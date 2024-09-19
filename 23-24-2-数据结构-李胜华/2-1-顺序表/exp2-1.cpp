#include "SqList.hpp"
#include <iostream>
using namespace std;

int main()
{
    SqList *L;
    ElemType a[] = {5, 8, 6, 1, 4};

    InitList(L);

    for (int i = 0; i < 5; i++)
        ListInsert(L, i + 1, a[i]);

    cout << "顺序表L: ";
    DispList(L);

    cout << "顺序表L的长度: " << ListLength(L) << endl;

    if (ListEmpty(L))
        cout << "顺序表L为空" << endl;
    else
        cout << "顺序表L不为空" << endl;

    ElemType e;
    GetElem(L, 3, e);
    cout << "顺序表L的第3个元素为: " << e << endl;

    cout << "元素4的位置为: " << LocateElem(L, 4) << endl;

    ListInsert(L, 4, 2);
    cout << "在第4个元素的位置插入元素2后的顺序表L: ";
    DispList(L);

    ElemType deletedElem;
    ListDelete(L, 3, deletedElem);
    cout << "删除第3个元素后的顺序表L: ";
    DispList(L);

    ListReverse(L);
    cout << "逆序后的顺序表L:";
    DispList(L);

    cout << "顺序表L中间位置的元素为:" << MidElem(L) << endl;

    cout << "顺序表L所有元素的和为:" << Sum_Recursion(L, L->length) << endl;

    MergeSort_Recursion(L, 0, L->length - 1);
    cout << "顺序表L排序结果:";
    DispList(L);

    DestroyList(L);

    return 0;
}