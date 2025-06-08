#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
using namespace std;

const int INF = numeric_limits<int>::max();

class HungarianAlgorithm {
public:
    HungarianAlgorithm(const vector<vector<int>>& costMatrix)
        : n(costMatrix.size()), cost(costMatrix), u(n + 1), v(n + 1), p(n + 1), way(n + 1) {}

    int solve(vector<int>& assignment) {
        for (int i = 1; i <= n; ++i) {
            p[0] = i;
            vector<int> minv(n + 1, INF);
            vector<bool> used(n + 1, false);
            int j0 = 0;
            do {
                used[j0] = true;
                int i0 = p[j0], delta = INF, j1 = 0;
                for (int j = 1; j <= n; ++j) {
                    if (!used[j]) {
                        int cur = cost[i0 - 1][j - 1] - u[i0] - v[j];
                        if (cur < minv[j]) {
                            minv[j] = cur;
                            way[j] = j0;
                        }
                        if (minv[j] < delta) {
                            delta = minv[j];
                            j1 = j;
                        }
                    }
                }
                for (int j = 0; j <= n; ++j) {
                    if (used[j]) {
                        u[p[j]] += delta;
                        v[j] -= delta;
                    } else {
                        minv[j] -= delta;
                    }
                }
                j0 = j1;
            } while (p[j0] != 0);
            do {
                int j1 = way[j0];
                p[j0] = p[j1];
                j0 = j1;
            } while (j0);
        }
        assignment.resize(n);
        for (int j = 1; j <= n; ++j) {
            assignment[p[j] - 1] = j - 1;
        }
        return -v[0];
    }

private:
    int n;
    vector<vector<int>> cost;
    vector<int> u, v, p, way;
};

int main() {
    // Example cost matrix (minimize total cost)
    vector<vector<int>> cost = {
        {9, 11, 14, 11, 7},
        {6, 15, 13, 13, 10},
        {12, 13, 6, 8, 8},
        {11, 9, 10, 12, 9},
        {7, 12, 14, 10, 14}
    };

    HungarianAlgorithm hungarian(cost);
    vector<int> assignment;
    int minCost = hungarian.solve(assignment);

    cout << "Minimum cost: " << minCost << endl;
    cout << "Assignments:" << endl;
    for (int i = 0; i < assignment.size(); ++i) {
        cout << "Worker " << i << " assigned to job " << assignment[i] << endl;
    }

    return 0;
}
