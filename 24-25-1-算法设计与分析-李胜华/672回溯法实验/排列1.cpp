#include <iostream>

using namespace std;

#define N 3
int b[N] = {0};
int x[N] = {0};

void dfs(int step)
{
    if (step == N)
    {
        for(int num : x)
            cout << num;
        cout << endl;

        return ;
    }

    for (int i = 0; i < N; ++i)
        if (b[i] == 0)
        {
            x[step] = i + 1, b[i] = 1;
            dfs(step + 1);
            b[i] = 0;
        }
}

int main()
{
    dfs(0);

    return 0;
}