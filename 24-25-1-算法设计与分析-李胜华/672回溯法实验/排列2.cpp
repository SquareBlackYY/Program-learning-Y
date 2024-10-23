#include <iostream>
#include <vector>

using namespace std;

#define N 3
vector<int> x(N);

void dfs(int step)
{
    if (step == N)
    {
        for (int num : x)
            cout << num;
        cout << endl;

        return;
    }

    for (int i = step; i < N; ++i)
    {
        swap(x[i], x[step]);
        dfs(step + 1);
        swap(x[i], x[step]);
    }

    return;
}

int main()
{
    for (int i = 0; i < N; ++i)
        x[i] = i + 1;

    dfs(0);

    return 0;
}