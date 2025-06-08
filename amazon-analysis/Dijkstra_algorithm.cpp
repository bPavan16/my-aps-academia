// Generic C++ implementation of Dijkstra's algorithm
#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

class DijkstraAlgorithm {
private:
    struct Edge {
        int destination;
        double weight;
        
        Edge(int dest, double w) : destination(dest), weight(w) {}
    };
    
    int vertices;
    std::vector<std::vector<Edge>> adjacencyList;

public:
    // Constructor
    DijkstraAlgorithm(int numVertices) : vertices(numVertices) {
        adjacencyList.resize(vertices);
    }
    
    // Add weighted edge to the graph
    void addEdge(int source, int destination, double weight) {
        adjacencyList[source].emplace_back(destination, weight);
    }
    
    // Dijkstra's shortest path algorithm
    std::pair<std::vector<double>, std::vector<int>> shortestPath(int source) {
        // Distance array initialized to infinity
        std::vector<double> distance(vertices, std::numeric_limits<double>::infinity());
        
        // Parent array for path reconstruction
        std::vector<int> parent(vertices, -1);
        
        // Visited array
        std::vector<bool> visited(vertices, false);
        
        // Priority queue: {distance, vertex}
        std::priority_queue<std::pair<double, int>, 
                          std::vector<std::pair<double, int>>,
                          std::greater<std::pair<double, int>>> pq;
        
        // Initialize source vertex
        distance[source] = 0.0;
        pq.push({0.0, source});
        
        while (!pq.empty()) {
            double currentDistance = pq.top().first;
            int currentVertex = pq.top().second;
            pq.pop();
            
            // Skip if already processed
            if (visited[currentVertex]) continue;
            visited[currentVertex] = true;
            
            // Process all adjacent vertices
            for (const Edge& edge : adjacencyList[currentVertex]) {
                int neighbor = edge.destination;
                double edgeWeight = edge.weight;
                
                // Relaxation step
                if (distance[currentVertex] + edgeWeight < distance[neighbor]) {
                    distance[neighbor] = distance[currentVertex] + edgeWeight;
                    parent[neighbor] = currentVertex;
                    pq.push({distance[neighbor], neighbor});
                }
            }
        }
        
        return {distance, parent};
    }
    
    // Reconstruct path from source to destination
    std::vector<int> getPath(const std::vector<int>& parent, int source, int destination) {
        std::vector<int> path;
        
        if (parent[destination] == -1 && destination != source) {
            return path;  // No path exists
        }
        
        int current = destination;
        while (current != -1) {
            path.push_back(current);
            current = parent[current];
        }
        
        std::reverse(path.begin(), path.end());
        return path;
    }
};
