#include <iostream>
#include <vector>

using namespace std;

#define N 3
vector<int> x(N);
int countNum = 0;
int sum = 0;

int SimpleSort(vector<int> x)
{
    int count = 0;

    for (int i = 0; i < N - 1; ++i)
    {
        int min = i;
        for (int j = i + 1; j < N; ++j)
            if (x[j] < x[min])
                min = j;
                
        if (i != min)
        {
            swap(x[i], x[min]);
            ++count;
        }
    }
        
    return count;
}

void dfs(int step)
{
    if (step == N)
    {
        ++countNum;
        sum += SimpleSort(x);
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

    cout << "平均步骤：" << sum << '/' << countNum << endl;

    return 0;
}