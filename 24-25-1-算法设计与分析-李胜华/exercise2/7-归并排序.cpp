#include <iostream>
using namespace std;

template <class Type>
void MergeSort(Type[], int, int);
template <class Type>
void Merge(Type[], int, int, int);

int main()
{
    // 使用自己编写的Merge函数
    int a[] = {3, 1, 8, 4, 2, 5};
    MergeSort(a, 0, sizeof(a) / sizeof(a[0]) - 1);

    cout << "排序结果:";
    for (int i : a)
        cout << " " << i;
    cout << endl;

    int b[] = {3, 1, 8, 4, 2, 5};
    
    // 使用 std::sort 进行排序
    sort(b, b + sizeof(b) / sizeof(b[0]));

    cout << "sort验证:";
    for (int i : b)
        cout << " " << i;
    cout << endl;

    return 0;
}

template <class Type>
void MergeSort(Type a[], int left, int right)
{
    if (left < right)
    {
        int mid = (left + right) / 2;
        MergeSort(a, left, mid);
        MergeSort(a, mid + 1, right);
        Merge(a, left, mid, right);
    }
}

template <class Type>
void Merge(Type a[], int l, int m, int r)
{
    int i = l, j = m + 1, k = 0;
    Type *b;
    b = new Type[r - l + 1];
    while (i <= m && j <= r)
        if (a[i] <= a[j])
            b[k++] = a[i++];
        else
            b[k++] = a[j++];
    while (i <= m)
        b[k++] = a[i++];
    while (j <= r)
        b[k++] = a[j++];
    for (i = 0; i < k; i++)
        a[l + i] = b[i];
    delete[] b;
}