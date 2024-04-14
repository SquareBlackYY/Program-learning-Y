#ifndef SQLIST_HPP
#define SQLIST_HPP

const int MaxSize = 100;

typedef int ElemType;
typedef struct
{
    ElemType data[MaxSize];
    int length;
} SqList;

void CreateList(SqList *&L, ElemType a[], int n);             // 张笑宇：通过数组创建线性表
void InitList(SqList *&L);                                    // 张笑宇：初始化线性表，构造一个空的线性表L
void DestroyList(SqList *&L);                                 // 曹  阳：销毁线性表，释放为线性表L分配的内存空间
bool ListEmpty(SqList *L);                                    // 曹  阳：判断线性表是否为空表，若L为空表，则返回真，否则返回假
int ListLength(SqList *L);                                    // 朱叶凯：求线性表的长度，返回L中元素的个数
void DispList(SqList *L);                                     // 朱叶凯：输出线性表，当线性表L不为空时顺序输出L中各元素值
bool GetElem(SqList *L, int i, ElemType &e);                  // 张正阳：按序号求线性表中元素，用e返回L中第i(1≤i≤n)个元素值
int LocateElem(SqList *L, ElemType e);                        // 张正阳：按元素值查找，返回L中第一个值为e相等的元素序号
bool ListInsert(SqList *&L, int i, ElemType e);               // 许靖轩：插入元素，在L的第i(1≤i≤n+1)个位置插入一个新元素e
bool ListDelete(SqList *&L, int i, ElemType &e);              // 许靖轩：删除元素，删除L的第i(1≤i≤n)个元素，并用e返回该元素值
ElemType Sum_Recursion(SqList *L, int n);                     // 许靖轩：非空线性表中元素之和（递归）
void MergeSort_Recursion(SqList *&L, int left, int right);    // 刘文俊：归并排序（递归）
void Merge_Recursion(SqList *&L, int low, int mid, int high); // 刘文俊：归并排序（递归）

#endif