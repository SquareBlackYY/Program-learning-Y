// 测试 CreateList
#include "SqList.hpp"
#include <iostream>
using namespace std;

int main()
{
    SqList *L;
    ElemType a[] = {5, 8, 6, 1, 4};
    CreateList(L, a, 5);
    cout << "线性表创建成功:" << endl;
    DispList(L);
    return 0;
}

// 测试 InitList
#include "SqList.hpp"
#include <iostream>
using namespace std;

int main()
{
    SqList *L;
    InitList(L);
    cout << "线性表初始化成功:" << endl;
    DispList(L);
    return 0;
}

// 测试 DestroyList
#include "SqList.hpp"
#include <iostream>
using namespace std;

int main()
{
    SqList *L;
    ElemType a[] = {5, 8, 6, 1, 4};
    CreateList(L, a, 5);
    DestroyList(L);
    cout << "线性表销毁成功:" << endl;
    return 0;
}

// 测试 ListEmpty
#include "SqList.hpp"
#include <iostream>
using namespace std;

int main()
{
    SqList *L;
    InitList(L);
    if (ListEmpty(L))
        cout << "线性表为空" << endl;
    else
        cout << "线性表不为空" << endl;
    return 0;
}

// 测试 ListLength
#include "SqList.hpp"
#include <iostream>
using namespace std;

int main()
{
    SqList *L;
    ElemType a[] = {5, 8, 6, 1, 4};
    CreateList(L, a, 5);
    cout << "线性表的长度为: " << ListLength(L) << endl;
    return 0;
}

// 测试 DispList
#include "SqList.hpp"
#include <iostream>
using namespace std;

int main()
{
    SqList *L;
    ElemType a[] = {5, 8, 6, 1, 4};
    CreateList(L, a, 5);
    cout << "线性表的元素为: ";
    DispList(L);
    return 0;
}

// 测试 GetElem
#include "SqList.hpp"
#include <iostream>
using namespace std;

int main()
{
    SqList *L;
    ElemType a[] = {5, 8, 6, 1, 4};
    CreateList(L, a, 5);
    ElemType e;
    int pos = 3;
    if (GetElem(L, pos, e))
        cout << "第" << pos << "个元素为: " << e << endl;
    else
        cout << "获取第" << pos << "个元素失败" << endl;
    return 0;
}

// 测试 LocateElem
#include "SqList.hpp"
#include <iostream>
using namespace std;

int main()
{
    SqList *L;
    ElemType a[] = {5, 8, 6, 1, 4};
    CreateList(L, a, 5);
    ElemType e = 6;
    int pos = LocateElem(L, e);
    if (pos != 0)
        cout << "元素" << e << "位于第" << pos << "个位置" << endl;
    else
        cout << "元素" << e << "不存在" << endl;
    return 0;
}

// 测试 ListInsert
#include "SqList.hpp"
#include <iostream>
using namespace std;

int main()
{
    SqList *L;
    ElemType a[] = {5, 8, 6, 1, 4};
    CreateList(L, a, 5);
    int pos = 3;
    ElemType e = 9;
    if (ListInsert(L, pos, e))
    {
        cout << "在第" << pos << "个位置插入元素" << e << "后，线性表为: ";
        DispList(L);
    }
    else
    {
        cout << "插入失败" << endl;
    }
    return 0;
}

// 测试 ListDelete
#include "SqList.hpp"
#include <iostream>
using namespace std;

int main()
{
    SqList *L;
    ElemType a[] = {5, 8, 6, 1, 4};
    CreateList(L, a, 5);
    int pos = 3;
    ElemType e;
    if (ListDelete(L, pos, e))
    {
        cout << "删除第" << pos << "个位置的元素" << e << "后，线性表为: ";
        DispList(L);
    }
    else
    {
        cout << "删除失败" << endl;
    }
    return 0;
}

// 测试 ListReverse
#include "SqList.hpp"
#include <iostream>
using namespace std;

int main()
{
    SqList *L;
    ElemType a[] = {5, 8, 6, 1, 4};
    CreateList(L, a, 5);
    ListReverse(L);
    cout << "线性表逆序后为: ";
    DispList(L);
    return 0;
}

// 测试 MidElem
#include "SqList.hpp"
#include <iostream>
using namespace std;

int main()
{
    SqList *L;
    ElemType a[] = {5, 8, 6, 1, 4};
    CreateList(L, a, 5);
    cout << "线性表中间位置的元素为: " << MidElem(L) << endl;
    return 0;
}

// 测试 Sum_Recursion
#include "SqList.hpp"
#include <iostream>
using namespace std;

int main()
{
    SqList *L;
    ElemType a[] = {1, 2, 3, 4, 5};
    CreateList(L, a, 5);
    cout << "线性表元素之和为: " << Sum_Recursion(L, ListLength(L)) << endl;
    return 0;
}

// 测试 MergeSort_Recursion
#include "SqList.hpp"
#include <iostream>
using namespace std;

int main()
{
    SqList *L;
    ElemType a[] = {38, 27, 43, 3, 9, 82, 10};
    CreateList(L, a, 7);
    MergeSort_Recursion(L, 0, ListLength(L) - 1);
    cout << "归并排序后的线性表为: ";
    DispList(L);
    return 0;
}
