#include <iostream>
#include <vector>
using namespace std;

class UnionFind {
    vector<int> parent, rank;
public:
    UnionFind(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; ++i)
            parent[i] = i;
    }

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]); // Path compression
        return parent[x];
    }

    bool unite(int x, int y) {
        int xr = find(x), yr = find(y);
        if (xr == yr) return false;
        if (rank[xr] < rank[yr]) parent[xr] = yr;
        else if (rank[xr] > rank[yr]) parent[yr] = xr;
        else { parent[yr] = xr; rank[xr]++; }
        return true;
    }

    bool connected(int x, int y) {
        return find(x) == find(y);
    }
};

int main() {
    int n = 5;
    UnionFind uf(n);

    uf.unite(0, 1);
    uf.unite(1, 2);
    uf.unite(3, 4);

    cout << "0 and 2 connected? " << (uf.connected(0, 2) ? "Yes" : "No") << endl;
    cout << "0 and 4 connected? " << (uf.connected(0, 4) ? "Yes" : "No") << endl;

    uf.unite(2, 4);

    cout << "0 and 4 connected after union? " << (uf.connected(0, 4) ? "Yes" : "No") << endl;

    return 0;
}
