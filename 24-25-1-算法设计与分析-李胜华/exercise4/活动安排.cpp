#include <iostream>

using namespace std;

typedef struct
{
    string st, ed;
    int no;
} activity ;

int cmp(activity x, activity y);

int GreedySelection(activity T[], int n, int A[]);

int main()
{
    activity *a;
    int n, i, *bx, bv;

    // 读入活动数
    freopen("input.txt", "r", stdin);
    cin >> n;

    a = new activity[n + 1];
    bx = new int[n + 1];

    // 读入活动数据
    for (int i = 1; i <= n; i++)
    {
        cin >> a[i].st >> a[i].ed;
        a[i].no = i;
    }

    fclose(stdin);

    // 最优解活动数
    bv = GreedySelection(a, n, bx);

    cout << "最优解：";
    for (int i = 1; i <= n; i++)
        if (bx[i])
        cout << i << ' ';
    cout << endl;

    cout << "最优解活动数：" << bv << endl;

    delete[] a;
    delete[] bx;

    return 0;
}

int cmp(activity x, activity y)
{
    return x.ed <= y.ed;
}

int GreedySelection(activity T[], int n, int A[])
{
    // 按结束时间从早到晚排序
    sort(T + 1, T + n + 1, cmp);

    A[T[1].no] = 1;

    int s = 1, j = 1;
    for (int i = 2; i <= n; i++)
    {
        if (T[i].st >= T[j].ed)
        {
            A[T[i].no] = 1;
            s++;
            j = i;
        }
        else
            A[T[i].no] = 0;
    }
    return s;
}