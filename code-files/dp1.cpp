#include <bits/stdc++.h>
using namespace std;
#define INF 1e9

int n;
vector<vector<int>> cost;
vector<int> dp;

int solve(int mask)
{
    if (mask == (1 << n) - 1)
        return 0;
    if (dp[mask] != -1)
        return dp[mask];

    int worker = __builtin_popcount(mask);
    int minCost = INF;

    for (int job = 0; job < n; job++)
    {
        if (!(mask & (1 << job)))
        {
            minCost = min(minCost, cost[worker][job] + solve(mask | (1 << job)));
        }
    }
    return dp[mask] = minCost;
}

int main()
{
    cin >> n;
    cost.assign(n, vector<int>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> cost[i][j];

    dp.assign(1 << n, -1);
    cout << solve(0) << endl;
    return 0;
}