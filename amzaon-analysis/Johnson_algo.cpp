// Generic C++ implementation of Johnson's algorithm
#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

class JohnsonAlgorithm {
private:
    struct Edge {
        int from, to;
        double weight;
        
        Edge(int f, int t, double w) : from(f), to(t), weight(w) {}
    };
    
    int vertices;
    std::vector<Edge> edges;
    std::vector<std::vector<std::pair<int, double>>> adjacencyList;
    const double INF = std::numeric_limits<double>::infinity();
    
    // Bellman-Ford algorithm for detecting negative cycles
    std::pair<bool, std::vector<double>> bellmanFord(int source, 
                                                      const std::vector<Edge>& edgeList) {
        std::vector<double> distance(vertices + 1, INF);
        distance[source] = 0.0;
        
        // Relax edges V-1 times
        for (int i = 0; i < vertices; i++) {
            for (const Edge& edge : edgeList) {
                if (distance[edge.from] != INF && 
                    distance[edge.from] + edge.weight < distance[edge.to]) {
                    distance[edge.to] = distance[edge.from] + edge.weight;
                }
            }
        }
        
        // Check for negative cycles
        for (const Edge& edge : edgeList) {
            if (distance[edge.from] != INF && 
                distance[edge.from] + edge.weight < distance[edge.to]) {
                return {false, {}};  // Negative cycle detected
            }
        }
        
        return {true, distance};
    }

public:
    // Constructor
    JohnsonAlgorithm(int numVertices) : vertices(numVertices) {
        adjacencyList.resize(vertices);
    }
    
    // Add weighted edge to the graph
    void addEdge(int from, int to, double weight) {
        edges.emplace_back(from, to, weight);
        adjacencyList[from].emplace_back(to, weight);
    }
    
    // Johnsons algorithm for all-pairs shortest paths
    std::pair<bool, std::vector<std::vector<double>>> allPairsShortestPaths() {
        // Step 1: Add a new vertex connected to all vertices with weight 0
        std::vector<Edge> modifiedEdges = edges;
        for (int i = 0; i < vertices; i++) {
            modifiedEdges.emplace_back(vertices, i, 0.0);  // New vertex has index 'vertices'
        }
        
        // Step 2: Run Bellman-Ford from the new vertex
        auto [hasNoCycle, h] = bellmanFord(vertices, modifiedEdges);
        
        if (!hasNoCycle) {
            return {false, {}};
        }
        
        // Step 3: Reweight edges and run Dijkstra from each vertex
        std::vector<std::vector<double>> allDistances(vertices);
        
        for (int source = 0; source < vertices; source++) {
            allDistances[source] = dijkstraWithReweighting(source, h);
        }
        
        return {true, allDistances};
    }
    
private:
    // Dijkstra with reweighted edges
    std::vector<double> dijkstraWithReweighting(int source, const std::vector<double>& h) {
        std::vector<double> distance(vertices, INF);
        std::vector<bool> visited(vertices, false);
        
        std::priority_queue<std::pair<double, int>, 
                          std::vector<std::pair<double, int>>,
                          std::greater<std::pair<double, int>>> pq;
        
        distance[source] = 0.0;
        pq.push({0.0, source});
        
        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();
            
            if (visited[u]) continue;
            visited[u] = true;
            
            for (const auto& [v, weight] : adjacencyList[u]) {
                // Use reweighted edge cost
                double reweightedCost = weight + h[u] - h[v];
                
                if (distance[u] + reweightedCost < distance[v]) {
                    distance[v] = distance[u] + reweightedCost;
                    pq.push({distance[v], v});
                }
            }
        }
        
        // Convert back to original weights
        for (int i = 0; i < vertices; i++) {
            if (distance[i] != INF) {
                distance[i] = distance[i] - h[source] + h[i];
            }
        }
        
        return distance;
    }
};
