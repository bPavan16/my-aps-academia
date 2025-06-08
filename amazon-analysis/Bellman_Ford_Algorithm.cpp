#include <iostream>
#include <vector>
#include <climits>
using namespace std;

struct Edge {
    int src, dest, weight;
};

class Graph {
    int V, E;
    vector<Edge> edges;

public:
    Graph(int V, int E) {
        this->V = V;
        this->E = E;
    }

    void addEdge(int u, int v, int w) {
        Edge edge = {u, v, w};
        edges.push_back(edge);
    }

    void bellmanFord(int src) {
        vector<int> dist(V, INT_MAX);
        dist[src] = 0;

        // Relax all edges |V| - 1 times
        for (int i = 0; i < V - 1; i++) {
            for (int j = 0; j < E; j++) {
                int u = edges[j].src;
                int v = edges[j].dest;
                int weight = edges[j].weight;
                
                if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                }
            }
        }

        // Check for negative-weight cycles
        for (int i = 0; i < E; i++) {
            int u = edges[i].src;
            int v = edges[i].dest;
            int weight = edges[i].weight;
            
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                cout << "Graph contains negative weight cycle" << endl;
                return;
            }
        }

        printSolution(dist, src);
    }

    void printSolution(vector<int>& dist, int src) {
        cout << "Vertex Distance from Source " << src << endl;
        for (int i = 0; i < V; i++) {
            if (dist[i] == INT_MAX) {
                cout << i << "\t\tINF" << endl;
            } else {
                cout << i << "\t\t" << dist[i] << endl;
            }
        }
    }
};

int main() {
    int V = 5;  // Number of vertices
    int E = 8;  // Number of edges
    Graph graph(V, E);

    // Add edges (source, destination, weight)
    graph.addEdge(0, 1, -1);
    graph.addEdge(0, 2, 4);
    graph.addEdge(1, 2, 3);
    graph.addEdge(1, 3, 2);
    graph.addEdge(1, 4, 2);
    graph.addEdge(3, 2, 5);
    graph.addEdge(3, 1, 1);
    graph.addEdge(4, 3, -3);

    int source = 0;
    cout << "Bellman-Ford Algorithm:" << endl;
    graph.bellmanFord(source);

    return 0;
}
