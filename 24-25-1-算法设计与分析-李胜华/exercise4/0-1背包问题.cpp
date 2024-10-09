#include <iostream>
#include <algorithm>
using namespace std;

/**
 * @brief 0-1背包问题
 * @param c 背包载重量
 * @param n 物品总数
 * @param w 物品重量数组
 * @param v 物品价值数组
 * @return 能装入背包的物品的最大总价值
 */
int BackPack(int c, int n, int *w, int *v)
{
    int dp[n][c + 1];

    // 初始化
    for (int i = 0; i <= c; ++i)
        dp[0][i] = w[0] > i ? 0 : v[0];

    // 动态规划
    for (int i = 1; i < n; ++i)
        for (int j = 0; j <= c; ++j)
            dp[i][j] = w[i] > j ? dp[i - 1][j] : max(dp[i - 1][j], dp[i - 1][j - w[i]] + v[i]);

    return dp[n - 1][c];
}


int main()
{
    int c = 6;
    int n = 3;
    int w[] = {1, 2, 5};
    int v[] = {2, 3, 4};

    int maxValue = BackPack(c, n, w, v);

    cout << "最大价值:" << maxValue << endl;
    return 0;
}