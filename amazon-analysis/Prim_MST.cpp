
#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <climits>
using namespace std;

// Graph represented as adjacency list: (vertex, weight)
typedef pair<int, int> pii;

void primMST(const vector<vector<pii>>& adj, int V) {
    vector<int> key(V, INT_MAX);
    vector<int> parent(V, -1);
    vector<bool> inMST(V, false);

    // Min-heap priority queue: (key, vertex)
    priority_queue<pii, vector<pii>, greater<pii>> pq;
    key[0] = 0;
    pq.push({0, 0});

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        if (inMST[u]) continue;
        inMST[u] = true;

        for (auto& [v, weight] : adj[u]) {
            if (!inMST[v] && weight < key[v]) {
                key[v] = weight;
                parent[v] = u;
                pq.push({key[v], v});
            }
        }
    }

    cout << "Edges in MST:\n";
    int totalWeight = 0;
    for (int v = 1; v < V; ++v) {
        if (parent[v] != -1) {
            cout << parent[v] << " - " << v << " : " << key[v] << endl;
            totalWeight += key[v];
        }
    }
    cout << "Total weight of MST: " << totalWeight << endl;
}

int main() {
    int V = 5;
    vector<vector<pii>> adj(V);

    // Example undirected weighted graph
    adj[0].push_back({1, 2});
    adj[1].push_back({0, 2});

    adj[0].push_back({3, 6});
    adj[3].push_back({0, 6});

    adj[1].push_back({2, 3});
    adj[2].push_back({1, 3});

    adj[1].push_back({3, 8});
    adj[3].push_back({1, 8});

    adj[1].push_back({4, 5});
    adj[4].push_back({1, 5});

    adj[2].push_back({4, 7});
    adj[4].push_back({2, 7});

    primMST(adj, V);

    return 0;
}
