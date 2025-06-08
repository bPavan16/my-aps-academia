
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

// Kahn's Algorithm for Topological Sort (BFS)
class Graph {
    int V;
    vector<vector<int>> adj;

public:
    Graph(int V) : V(V), adj(V) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
    }

    void topologicalSort() {
        vector<int> in_degree(V, 0);

        // Compute in-degree of each vertex
        for (int u = 0; u < V; u++)
            for (int v : adj[u])
                in_degree[v]++;

        queue<int> q;
        for (int i = 0; i < V; i++)
            if (in_degree[i] == 0)
                q.push(i);

        int cnt = 0;
        vector<int> topo_order;

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            topo_order.push_back(u);

            for (int v : adj[u]) {
                if (--in_degree[v] == 0)
                    q.push(v);
            }
            cnt++;
        }

        if (cnt != V) {
            cout << "There exists a cycle in the graph\n";
            return;
        }

        cout << "Topological Sort: ";
        for (int v : topo_order)
            cout << v << " ";
        cout << endl;
    }
};

int main() {
    // Example usage
    Graph g(6);
    g.addEdge(5, 2);
    g.addEdge(5, 0);
    g.addEdge(4, 0);
    g.addEdge(4, 1);
    g.addEdge(2, 3);
    g.addEdge(3, 1);

    g.topologicalSort();

    return 0;
}
