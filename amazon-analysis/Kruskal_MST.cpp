
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Edge {
    int u, v, weight;
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

class DSU {
    vector<int> parent, rank;
public:
    DSU(int n) : parent(n), rank(n, 0) {
        for(int i = 0; i < n; ++i) parent[i] = i;
    }
    int find(int x) {
        if(parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }
    bool unite(int x, int y) {
        int xr = find(x), yr = find(y);
        if(xr == yr) return false;
        if(rank[xr] < rank[yr]) parent[xr] = yr;
        else if(rank[xr] > rank[yr]) parent[yr] = xr;
        else { parent[yr] = xr; rank[xr]++; }
        return true;
    }
};

int main() {
    int V = 4; // Number of vertices
    vector<Edge> edges = {
        {0, 1, 10},
        {0, 2, 6},
        {0, 3, 5},
        {1, 3, 15},
        {2, 3, 4}
    };

    sort(edges.begin(), edges.end());
    DSU dsu(V);
    vector<Edge> mst;
    int mst_weight = 0;

    for(const Edge& e : edges) {
        if(dsu.unite(e.u, e.v)) {
            mst.push_back(e);
            mst_weight += e.weight;
        }
    }

    cout << "Edges in MST:\n";
    for(const Edge& e : mst)
        cout << e.u << " - " << e.v << " : " << e.weight << endl;
    cout << "Total weight of MST: " << mst_weight << endl;

    return 0;
}
